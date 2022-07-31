// Cube_Game, All rights reserved.

#include "CGGameMode.h"
#include "CGGameInstance.h"
#include "Player/CGPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/CGHUDGame.h"
#include "Settings/CGGameUserSettings.h"

constexpr static int32 CountdownTimerRate = 1;
constexpr static int32 MaxMultiplier = 8;
constexpr static float SpeedUpHintDelay = 0.3f;

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
    if (SpeedToAdd >= 1)    //
    {
        AddSpeed(static_cast<int32>(SpeedToAdd));
    }

    AddScore(ScoreToAdd);
}

void ACGGameMode::ShowPopUpHint(const FHintData& HintData)
{
    SetPause(EGameState::PopUpHint);

    OnShowPopUpHintSignature.Broadcast(HintData);
}

void ACGGameMode::GameOver()
{
    SetPause(EGameState::GameOver);
}

void ACGGameMode::StartPlay()
{
    Super::StartPlay();

    SetGameState(EGameState::Game);
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACGGameMode::OnCountdown, CountdownTimerRate, true);

    FormatHints();
    ShowGameplayHint(EHintType::Startup, StartupHintDelay);
}

bool ACGGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    return Super::SetPause(PC, CanUnpauseDelegate);    // To be able to overload the function.
}

bool ACGGameMode::SetPause(EGameState NewGameState)
{
    if (!SetPause(GetWorld()->GetFirstPlayerController()))
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

    SetupGameMode();
}

void ACGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorldTimerManager().ClearTimer(DelayHintTimerHandle);    // Cos the timer starts lambda.
}

void ACGGameMode::SetupGameMode()
{
    Time = GetDifficultyVlues().InitialTime;

    OnMultiplierChanged.AddUObject(this, &ThisClass::OnShowMultiplierHint);
    OnLowTime.AddUObject(this, &ThisClass::OnShowLowTimeHint);
    OnSpeedChanged.AddUObject(this, &ThisClass::OnShowSpeedUpHint);

    if (const auto GameUserSettings = UCGGameUserSettings::Get())
    {
        HintsMap = GameUserSettings->GetHintsStatus().HintsMap;

        GameUserSettings->OnHintsStatusChanged.AddUObject(this, &ThisClass::OnHintsStatusChanged);
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
    if (!HintsMap.Contains(HintType))
        return;

    if (!HintsMap[HintType])
        return;

    if (!GameplayHintsMap.Contains(HintType))
        return;

    if (Delay)
    {
        GetWorldTimerManager().SetTimer(    //
            DelayHintTimerHandle,           //
            [=]()
            {
                this->ShowPopUpHint(GameplayHintsMap[HintType]);
            },
            Delay,     //
            false);    //
    }
    else
    {
        ShowPopUpHint(GameplayHintsMap[HintType]);
    }

    HintsMap[HintType] = false;

    if (const auto GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SetGameplayHintsStatus(HintsMap);
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
    ShowGameplayHint(EHintType::LowTime);
}

void ACGGameMode::OnShowSpeedUpHint(int32 NewSpeed)
{
    ShowGameplayHint(EHintType::SpeedUp, SpeedUpHintDelay);
}

void ACGGameMode::OnHintsStatusChanged(const FHintsStatus& NewHintsStatus)
{
    HintsMap = NewHintsStatus.HintsMap;
}

void ACGGameMode::AddTime(int32 TimeToAdd)
{
    Time = UKismetMathLibrary::Max(Time + TimeToAdd, 0);
    OnTimeChanged.Broadcast(Time);

    if (Time == 0)
    {
        GameOver();
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
