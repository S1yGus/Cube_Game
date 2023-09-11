// Cube_Game, All rights reserved.

#include "CGGameMode.h"
#include "CGGameInstance.h"
#include "Player/CGPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/CGHUDGame.h"
#include "Settings/CGGameUserSettings.h"
#include "Player/Components/CGBonusComponent.h"

constexpr static float CountdownTimerRate{1};
constexpr static float GameplayHintsDelay{0.2f};

ACGGameMode::ACGGameMode()
{
    DefaultPawnClass = ACGPlayer::StaticClass();
    HUDClass = ACGHUDGame::StaticClass();
}

int32 ACGGameMode::GetCubeSpeed() const
{
    if (const auto DifficultyData = GetDifficultyData())
    {
        return FMath::GetMappedRangeValueClamped(SpeedRange, DifficultyData->CubesSpeedRange, static_cast<float>(GameSpeed));
    }

    return 0;
}

const FDifficulty* ACGGameMode::GetDifficultyData() const
{
    if (const auto* GameInstance = GetGameInstance<UCGGameInstance>(); GameInstance && DifficultyMap.Contains(GameInstance->GetDifficulty()))
    {
        return &DifficultyMap[GameInstance->GetDifficulty()];
    }

    return nullptr;
}

void ACGGameMode::ChangeGameTime(ECubeType CubeType)
{
    if (const FDifficulty* DifficultyData = GetDifficultyData(); DifficultyData && DifficultyData->TimeChangeMap.Contains(CubeType))
    {
        AddTime(DifficultyData->TimeChangeMap[CubeType]);
    }
}

void ACGGameMode::ChangeGameSpeed(ECubeType CubeType)
{
    if (const FDifficulty* DifficultyData = GetDifficultyData(); DifficultyData && DifficultyData->SpeedChangeMap.Contains(CubeType))
    {
        AddGameSpeed(DifficultyData->SpeedChangeMap[CubeType]);
    }
}

void ACGGameMode::ChangeScore(ECubeType CubeType)
{
    ChangeMultiplier(CubeType);

    if (const FDifficulty* DifficultyData = GetDifficultyData(); DifficultyData && DifficultyData->ScoreChangeMap.Contains(CubeType))
    {
        const int32 ScoreRemains = Score % DifficultyData->ScoreToSpeedUp;
        const int32 ScoreToAdd = Multiplier * DifficultyData->ScoreChangeMap[CubeType];
        if (ScoreToAdd > 0)
        {
            if (const int32 SpeedToAdd = (ScoreRemains + ScoreToAdd) / DifficultyData->ScoreToSpeedUp)
            {
                AddGameSpeed(SpeedToAdd);
            }
        }

        AddScore(ScoreToAdd);
    }
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
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ThisClass::OnCountdown, CountdownTimerRate, true);

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

    if (const FDifficulty* DifficultyData = GetDifficultyData())
    {
        GameTime = DifficultyData->InitialTime;
    }
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

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        CachedHintsStatusMap = GameUserSettings->GetHintsStatus().HintsMap;
        GameUserSettings->OnHintsStatusChanged.AddUObject(this, &ThisClass::OnHintsStatusChanged);
    }

    if (const APawn* PlayerPawn = (GetWorld() && GetWorld()->GetFirstPlayerController()) ? GetWorld()->GetFirstPlayerController()->GetPawn() : nullptr)
    {
        if (auto* BonusComponent = PlayerPawn->FindComponentByClass<UCGBonusComponent>())
        {
            BonusComponent->OnBonusCharged.AddUObject(this, &ThisClass::OnShowBonusChargedHint);
        }
    }
}

void ACGGameMode::OnCountdown()
{
    AddTime(-CountdownTimerRate);

    if (GameTime < LowTimeThreshold)
    {
        OnLowTime.Broadcast();
    }
}

void ACGGameMode::ShowGameplayHint(EHintType HintType, float Delay)
{
    if (!CachedHintsStatusMap.Contains(HintType))
        return;

    if (!CachedHintsStatusMap[HintType])
        return;

    if (!GameplayHintsMap.Contains(HintType))
        return;

    if (Delay)
    {
        bShowingHint = true;
        GetWorldTimerManager().SetTimer(    //
            DelayHintTimerHandle,           //
            [this, &HintType]()
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

    CachedHintsStatusMap[HintType] = false;

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SetGameplayHintsStatus(CachedHintsStatusMap);
    }
}

void ACGGameMode::FormatHints()
{
    // Format SpeedUp hint.
    if (GameplayHintsMap.Contains(EHintType::SpeedUp) && GameplayHintsMap[EHintType::SpeedUp].HintText.ToString().Contains("{0}"))
    {
        FStringFormatOrderedArguments SpeedUpHintArg;
        if (const auto DifficultyData = GetDifficultyData())
        {
            SpeedUpHintArg.Add(GetDifficultyData()->ScoreToSpeedUp);
            GameplayHintsMap[EHintType::SpeedUp].HintText = FText::FromString(FString::Format(*GameplayHintsMap[EHintType::SpeedUp].HintText.ToString(), SpeedUpHintArg));
        }
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
    CachedHintsStatusMap = NewHintsStatus.HintsMap;
}

void ACGGameMode::AddTime(int32 TimeToAdd)
{
    GameTime = UKismetMathLibrary::Max(GameTime + TimeToAdd, 0);
    OnTimeChanged.Broadcast(GameTime);

    if (GameTime == 0)
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

void ACGGameMode::AddGameSpeed(int32 SpeedToAdd)
{
    GameSpeed = UKismetMathLibrary::Max(GameSpeed + SpeedToAdd, 1);
    OnSpeedChanged.Broadcast(GameSpeed);
}

void ACGGameMode::AddScore(int32 ScoreToAdd)
{
    Score = UKismetMathLibrary::Max(Score + ScoreToAdd, 0);
    OnScoreChanged.Broadcast(Score, ScoreToAdd, Multiplier);
}

void ACGGameMode::ChangeMultiplier(ECubeType CubeType)
{

    if (const FDifficulty* DifficultyData = GetDifficultyData();    //
        DifficultyData                                              //
        && DifficultyData->ScoreChangeMap.Contains(CubeType)        //
        && DifficultyData->ScoreChangeMap[CubeType] > 0             //
        && CubeType == PreviousCubeType)
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
