// Cube_Game, All rights reserved.

#include "CGGameMode.h"
#include "CGGameInstance.h"
#include "Player/CGPlayerController.h"
#include "Player/CGPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/CGHUDGame.h"
#include "Saves/CGSettingsSave.h"

constexpr static int32 CountdownTimerRate = 1;
const static FVector2D SpeedRange{1.0f, 999.0f};
constexpr static int32 MaxMultiplier = 8;
constexpr static int32 StartupHintTimerRate = 2;

ACGGameMode::ACGGameMode()
{
    PlayerControllerClass = ACGPlayerController::StaticClass();
    DefaultPawnClass = ACGPlayer::StaticClass();
    HUDClass = ACGHUDGame::StaticClass();
}

void ACGGameMode::StartPlay()
{
    Super::StartPlay();

    SetGameState(EGameState::Game);

    GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ACGGameMode::OnCountdown, CountdownTimerRate, true);
    GetWorld()->GetTimerManager().SetTimer(StartupHintTimerHandle, this, &ACGGameMode::OnShowStartupHint, StartupHintTimerRate);
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

void ACGGameMode::ChangeTime(ECubeType CubeType)
{
    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues || !DifficultyVlues->TimeChangeMap.Contains(CubeType))
        return;

    AddTime(DifficultyVlues->TimeChangeMap[CubeType]);
}

void ACGGameMode::ChangeSpeed(ECubeType CubeType)
{
    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues || !DifficultyVlues->SpeedChangeMap.Contains(CubeType))
        return;

    AddSpeed(DifficultyVlues->SpeedChangeMap[CubeType]);
}

void ACGGameMode::ChangeScore(ECubeType CubeType)
{
    ChangeMultiplier(CubeType);

    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues || !DifficultyVlues->ScoreChangeMap.Contains(CubeType))
        return;

    const auto ScoreRemains = Score % DifficultyVlues->ScoreToSpeedUp;
    const auto ScoreToAdd = Multiplier * DifficultyVlues->ScoreChangeMap[CubeType];
    const auto SpeedToAdd = (ScoreRemains + ScoreToAdd) / DifficultyVlues->ScoreToSpeedUp;
    if (SpeedToAdd >= 1)
    {
        AddSpeed(static_cast<int32>(SpeedToAdd));
    }

    AddScore(ScoreToAdd);
}

int32 ACGGameMode::GetCubeSpeed() const
{
    const auto DifficultyVlues = GetDifficultyVlues();
    const auto CubesSpeedRange = DifficultyVlues ? DifficultyVlues->CubesSpeedRange : FDifficulty{}.CubesSpeedRange;

    return FMath::GetMappedRangeValueClamped(SpeedRange, CubesSpeedRange, static_cast<float>(Speed));
}

const FDifficulty* ACGGameMode::GetDifficultyVlues() const
{
    const auto GameInstance = GetGameInstance<UCGGameInstance>();
    if (!GameInstance)
        return nullptr;

    if (!DifficultyMap.Contains(GameInstance->GetDifficulty()))
        return nullptr;

    return &DifficultyMap[GameInstance->GetDifficulty()];
}

void ACGGameMode::ShowPopUpHint(const FPopUpHint& PopUpHint)
{
    SetGameState(EGameState::PopUpHint);
    SetPause(GetWorld()->GetFirstPlayerController());

    OnShowPopUpHintSignature.Broadcast(PopUpHint);
}

void ACGGameMode::GameOver()
{
    SetGameState(EGameState::GameOver);
    SetPause(GetWorld()->GetFirstPlayerController());
}

void ACGGameMode::SetupGameMode()
{
    const auto DifficultyVlues = GetDifficultyVlues();

    Time = DifficultyVlues ? DifficultyVlues->InitialTime : FDifficulty{}.InitialTime;
}

void ACGGameMode::OnCountdown()
{
    AddTime(-CountdownTimerRate);

    if (Time < LowTimeThreshold)
    {
        OnLowTime.Broadcast();
    }
}

void ACGGameMode::OnShowStartupHint()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstance)
        return;

    const auto SettingsSave = GameInstance->GetSettingsSave();
    if (!SettingsSave)
        return;

    auto GameSettings = SettingsSave->GetGameSettings();
    if (!GameSettings.Hints.HintsMap.Contains(EHint::Startup))
        return;

    if (!GameSettings.Hints.HintsMap[EHint::Startup])
        return;

    if (!PopUpHintsMap.Contains(EHint::Startup))
        return;

    ShowPopUpHint(PopUpHintsMap[EHint::Startup]);

    GameSettings.Hints.HintsMap[EHint::Startup] = false;
    GameInstance->SetGameSettings(GameSettings);
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
    if (!DifficultyVlues)
        return;

    if (DifficultyVlues->ScoreChangeMap.Contains(CubeType) && DifficultyVlues->ScoreChangeMap[CubeType] > 0 && CubeType == PreviousCubeType)
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
