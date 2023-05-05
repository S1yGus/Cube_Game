// Cube_Game, All rights reserved.

#include "CGGameMode.h"
#include "CGGameInstance.h"
#include "Player/CGPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/CGHUDGame.h"
#include "Settings/CGGameUserSettings.h"
#include "Player/Components/CGBonusComponent.h"

constexpr static int32 CountdownTimerRate = 1;
constexpr static float GameplayHintsDelay = 0.2f;

ACGGameMode::ACGGameMode()
{
    DefaultPawnClass = ACGPlayer::StaticClass();
    HUDClass = ACGHUDGame::StaticClass();
}

int32 ACGGameMode::GetCubeSpeed() const
{
    return FMath::GetMappedRangeValueClamped(SpeedRange, GetDifficultyVlues().CubesSpeedRange, static_cast<float>(Speed));
}

const FDifficulty& ACGGameMode::GetDifficultyVlues() const
{
    const auto GameInstance = GetGameInstance<UCGGameInstance>();
    if (GameInstance && DifficultyMap.Contains(GameInstance->GetDifficulty()))
    {
        return DifficultyMap[GameInstance->GetDifficulty()];
    }

    static const auto Difficulty = FDifficulty{};
    return Difficulty;
}

void ACGGameMode::ChangeTime(ECubeType CubeType)
{
    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues.TimeChangeMap.Contains(CubeType))
        return;

    AddTime(DifficultyVlues.TimeChangeMap[CubeType]);
}

void ACGGameMode::ChangeSpeed(ECubeType CubeType)
{
    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues.SpeedChangeMap.Contains(CubeType))
        return;

    AddSpeed(DifficultyVlues.SpeedChangeMap[CubeType]);
}

void ACGGameMode::ChangeScore(ECubeType CubeType)
{
    ChangeMultiplier(CubeType);

    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues.ScoreChangeMap.Contains(CubeType))
        return;

    const auto ScoreRemains = Score % DifficultyVlues.ScoreToSpeedUp;
    const auto ScoreToAdd = Multiplier * DifficultyVlues.ScoreChangeMap[CubeType];
    const auto SpeedToAdd = (ScoreRemains + ScoreToAdd) / DifficultyVlues.ScoreToSpeedUp;
    if (SpeedToAdd >= 1)
    {
        AddSpeed(static_cast<int32>(SpeedToAdd));
    }

    AddScore(ScoreToAdd);
}

void ACGGameMode::ShowPopUpHint(const FHintData& HintData)
{
    bShowingHint = true;
    SetPause(EGameState::PopUpHint);
    OnShowPopUpHint.Broadcast(HintData);
}

void ACGGameMode::GameOver()
{
    SetPause(EGameState::GameOver);
}

void ACGGameMode::StartPlay()
{
    Super::StartPlay();

    SetupGameMode();

    SetGameState(EGameState::Game);
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACGGameMode::OnCountdown, CountdownTimerRate, true);

    ShowGameplayHint(EHintType::Startup, StartupHintDelay);
}

bool ACGGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    return Super::SetPause(PC, CanUnpauseDelegate);    // To be able to overload the function.
}

bool ACGGameMode::SetPause(EGameState NewGameState)
{
    if (!GetWorld() || !SetPause(GetWorld()->GetFirstPlayerController()))
        return false;

    SetGameState(NewGameState);
    return true;
}

bool ACGGameMode::ClearPause()
{
    if (!Super::ClearPause())
        return false;

    SetGameState(EGameState::Game);
    return true;
}

void ACGGameMode::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    Time = GetDifficultyVlues().InitialTime;
}

void ACGGameMode::SetGameState(EGameState NewGameState)
{
    Super::SetGameState(NewGameState);

    CheckFlags(NewGameState);
}

void ACGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorldTimerManager().ClearTimer(DelayHintTimerHandle);    // Cos the timer starts lambda.
}

void ACGGameMode::SetupGameMode()
{
    FormatHints();

    OnMultiplierChanged.AddUObject(this, &ThisClass::OnShowMultiplierHint);
    OnLowTime.AddUObject(this, &ThisClass::OnShowLowTimeHint);
    OnSpeedChanged.AddUObject(this, &ThisClass::OnShowSpeedUpHint);

    if (const auto GameUserSettings = UCGGameUserSettings::Get())
    {
        HintsStatusMap = GameUserSettings->GetHintsStatus().HintsMap;
        GameUserSettings->OnHintsStatusChanged.AddUObject(this, &ThisClass::OnHintsStatusChanged);
    }

    if (const auto PlayerPawn = (GetWorld() && GetWorld()->GetFirstPlayerController()) ? GetWorld()->GetFirstPlayerController()->GetPawn() : nullptr)
    {
        if (const auto BonusComponent = PlayerPawn->FindComponentByClass<UCGBonusComponent>())
        {
            BonusComponent->OnBonusCharged.AddUObject(this, &ThisClass::OnShowBonusChargedHint);
        }
    }
}

void ACGGameMode::OnCountdown()
{
    AddTime(-CountdownTimerRate);

    if (Time < LowTimeThreshold)
    {
        OnLowTime.Broadcast();
    }
}

void ACGGameMode::ShowGameplayHint(EHintType HintType, float Delay)
{
    if (!HintsStatusMap.Contains(HintType))
        return;

    if (!HintsStatusMap[HintType])
        return;

    if (!GameplayHintsMap.Contains(HintType))
        return;

    if (Delay)
    {
        bShowingHint = true;
        GetWorldTimerManager().SetTimer(    //
            DelayHintTimerHandle,           //
            [=]()
            {
                ShowPopUpHint(GameplayHintsMap[HintType]);
            },
            Delay,     //
            false);    //
    }
    else
    {
        ShowPopUpHint(GameplayHintsMap[HintType]);
    }

    HintsStatusMap[HintType] = false;

    if (const auto GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SetGameplayHintsStatus(HintsStatusMap);
    }
}

void ACGGameMode::FormatHints()
{
    // Format SpeedUp hint.
    if (GameplayHintsMap.Contains(EHintType::SpeedUp) && GameplayHintsMap[EHintType::SpeedUp].HintText.ToString().Contains("{0}"))
    {
        FStringFormatOrderedArguments SpeedUpHintArg;
        SpeedUpHintArg.Add(GetDifficultyVlues().ScoreToSpeedUp);
        GameplayHintsMap[EHintType::SpeedUp].HintText = FText::FromString(FString::Format(*GameplayHintsMap[EHintType::SpeedUp].HintText.ToString(), SpeedUpHintArg));
    }
}

void ACGGameMode::OnShowMultiplierHint(ECubeType CubeType, int32 CurrentMultiplier)
{
    ShowGameplayHint(EHintType::Multiplier);
}

void ACGGameMode::OnShowLowTimeHint()
{
    ShowGameplayHint(EHintType::LowTime, GameplayHintsDelay);
}

void ACGGameMode::OnShowSpeedUpHint(int32 NewSpeed)
{
    ShowGameplayHint(EHintType::SpeedUp, GameplayHintsDelay);
}

void ACGGameMode::OnShowBonusChargedHint(bool IsCharged)
{
    ShowGameplayHint(EHintType::BonusCharged);
}

void ACGGameMode::OnHintsStatusChanged(const FHintsStatus& NewHintsStatus)
{
    HintsStatusMap = NewHintsStatus.HintsMap;
}

void ACGGameMode::AddTime(int32 TimeToAdd)
{
    Time = UKismetMathLibrary::Max(Time + TimeToAdd, 0);
    OnTimeChanged.Broadcast(Time);

    if (Time == 0)
    {
        if (bShowingHint)
        {
            bGameOver = true;
        }
        else
        {
            GameOver();
        }
    }
}

void ACGGameMode::AddSpeed(int32 SpeedToAdd)
{
    Speed = UKismetMathLibrary::Max(Speed + SpeedToAdd, 1);
    OnSpeedChanged.Broadcast(Speed);
}

void ACGGameMode::AddScore(int32 ScoreToAdd)
{
    Score = UKismetMathLibrary::Max(Score + ScoreToAdd, 0);
    OnScoreChanged.Broadcast(Score, ScoreToAdd, Multiplier);
}

void ACGGameMode::ChangeMultiplier(ECubeType CubeType)
{
    const auto DifficultyVlues = GetDifficultyVlues();
    if (DifficultyVlues.ScoreChangeMap.Contains(CubeType) && DifficultyVlues.ScoreChangeMap[CubeType] > 0 && CubeType == PreviousCubeType)
    {
        if (Multiplier + 1 <= MaxMultiplier)
        {
            ++Multiplier;
            OnMultiplierChanged.Broadcast(CubeType, Multiplier);
        }
    }
    else if (Multiplier != 1)
    {
        Multiplier = 1;
        OnMultiplierChanged.Broadcast(CubeType, Multiplier);
    }

    PreviousCubeType = CubeType;
}

void ACGGameMode::CheckFlags(EGameState NewGameState)
{
    // If timer active then the hint is already marked as shown and should really be shown.
    if (NewGameState == EGameState::Game && !GetWorldTimerManager().IsTimerActive(DelayHintTimerHandle))
    {
        bShowingHint = false;
    }

    if (NewGameState == EGameState::Game && bGameOver && !bShowingHint)
    {
        GameOver();
    }
}
