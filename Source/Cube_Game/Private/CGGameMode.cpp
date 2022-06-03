// Cube_Game, All rights reserved.

#include "CGGameMode.h"
#include "CGGameInstance.h"
#include "Player/CGPlayerController.h"
#include "Player/CGPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

constexpr static int32 CountdownTimerRate = 1;
const static FVector2D SpeedRange{1.0f, 999.0f};
constexpr static int32 MaxMultiplier = 8;

ACGGameMode::ACGGameMode()
{
    PlayerControllerClass = ACGPlayerController::StaticClass();
    DefaultPawnClass = ACGPlayer::StaticClass();
}

void ACGGameMode::StartPlay()
{
    Super::StartPlay();

    SetupGameMode();
    SetGameState(EGameState::InGame);

    GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ACGGameMode::OnCountdown, CountdownTimerRate, true);
}

bool ACGGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    if (!Super::SetPause(PC, CanUnpauseDelegate))
        return false;

    SetGameState(EGameState::Pause);
    return true;
}

bool ACGGameMode::ClearPause()
{
    if (!Super::ClearPause())
        return false;

    SetGameState(EGameState::InGame);
    return true;
}

void ACGGameMode::ChangeTime(ECubeType CubeType)
{
    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues || !DifficultyVlues->TimeChangeMap.Contains(CubeType))
        return;

    SetTime(Time + DifficultyVlues->TimeChangeMap[CubeType]);
}

void ACGGameMode::ChangeSpeed(ECubeType CubeType)
{
    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues || !DifficultyVlues->SpeedChangeMap.Contains(CubeType))
        return;

    SetSpeed(Speed + DifficultyVlues->SpeedChangeMap[CubeType]);
}

void ACGGameMode::ChangeSpeed(int32 NewSpeed)
{
    SetSpeed(NewSpeed);
}

void ACGGameMode::ChangeScore(ECubeType CubeType)
{
    ChangeMultiplier(CubeType);

    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues)
        return;

    if (!DifficultyVlues->ScoreChangeMap.Contains(CubeType))
        return;

    SetScore(Score + Multiplier * DifficultyVlues->ScoreChangeMap[CubeType]);

    if (Score / DifficultyVlues->ScoreToSpeedUp > Speed)
    {
        ChangeSpeed(Score / DifficultyVlues->ScoreToSpeedUp);
    }
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

void ACGGameMode::GameOver()
{
    SetGameState(EGameState::GameOver);
}

void ACGGameMode::SetupGameMode()
{
    const auto DifficultyVlues = GetDifficultyVlues();

    Time = DifficultyVlues ? DifficultyVlues->InitialTime : FDifficulty{}.InitialTime;
}

void ACGGameMode::OnCountdown()
{
    SetTime(-CountdownTimerRate);

    if (Time < LowTimeThreshold)
    {
        OnLowTime.Broadcast();
        UGameplayStatics::PlaySound2D(GetWorld(), LowTimeSound);
    }
}

void ACGGameMode::SetTime(int32 NewTime)
{
    Time = UKismetMathLibrary::Max(NewTime, 0);
    OnTimeChanged.Broadcast(Time);

    if (Time == 0)
    {
        GameOver();
    }
}

void ACGGameMode::SetSpeed(int32 NewSpeed)
{
    Speed = UKismetMathLibrary::Max(NewSpeed, 1);
    OnSpeedChanged.Broadcast(Speed);
}

void ACGGameMode::SetScore(int32 NewScore)
{
    Score = UKismetMathLibrary::Max(NewScore, 0);
    OnScoreChanged.Broadcast(Score);
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
