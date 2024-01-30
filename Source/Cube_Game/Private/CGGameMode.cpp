// Cube_Game, All rights reserved.

#include "CGGameMode.h"
#include "CGGameInstance.h"
#include "Player/CGPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/CGHUDGame.h"
#include "Settings/CGGameUserSettings.h"
#include "Player/Components/CGBonusComponent.h"
#include "Player/CGPlayerController.h"
#include "Gameplay/CGFieldActor.h"
#include "Player/CGPlayer.h"
#include "NiagaraFunctionLibrary.h"

constexpr static float CountdownTimerRate{1.0f};
constexpr static double VFXSpawnZOffset{-100.0};

static TQueue<EHintType> HintsQueue;

ACGGameMode::ACGGameMode()
{
    PlayerControllerClass = ACGPlayerController::StaticClass();
    DefaultPawnClass = ACGPlayer::StaticClass();
    HUDClass = ACGHUDGame::StaticClass();
}

int32 ACGGameMode::GetCubeSpeed() const
{
    if (const FDifficulty* DifficultyData = GetDifficultyData())
    {
        return FMath::GetMappedRangeValueClamped(SpeedRange, DifficultyData->CubesSpeedRange, static_cast<float>(GameSpeed));
    }

    return 0;
}

const FDifficulty* ACGGameMode::GetDifficultyData() const
{
    if (const auto* GameUserSettings = UCGGameUserSettings::Get();    //
        GameUserSettings && DifficultyMap.Contains(GameUserSettings->GetCurrentDifficulty()))
    {
        return &DifficultyMap[GameUserSettings->GetCurrentDifficulty()];
    }

    return nullptr;
}

bool ACGGameMode::IsCubeNegative(ECubeType CubeType) const
{
    if (const FDifficulty* DifficultyData = GetDifficultyData())
    {
        if (DifficultyData->ScoreChangeMap.Contains(CubeType))
        {
            if (DifficultyData->ScoreChangeMap[CubeType] < 0)
            {
                return true;
            }
        }

        if (DifficultyData->TimeChangeMap.Contains(CubeType))
        {
            if (DifficultyData->TimeChangeMap[CubeType] < 0)
            {
                return true;
            }
        }

        if (DifficultyData->SpeedChangeMap.Contains(CubeType))
        {
            if (DifficultyData->SpeedChangeMap[CubeType] > 0)
            {
                return true;
            }
        }
    }

    return false;
}

void ACGGameMode::ChangeGameTime(ECubeType CubeType)
{
    if (const FDifficulty* DifficultyData = GetDifficultyData();    //
        DifficultyData && DifficultyData->TimeChangeMap.Contains(CubeType))
    {
        AddTime(DifficultyData->TimeChangeMap[CubeType]);
    }
}

void ACGGameMode::ChangeGameSpeed(ECubeType CubeType)
{
    if (const FDifficulty* DifficultyData = GetDifficultyData();    //
        DifficultyData && DifficultyData->SpeedChangeMap.Contains(CubeType))
    {
        AddGameSpeed(DifficultyData->SpeedChangeMap[CubeType]);
    }
}

void ACGGameMode::ChangeScore(ECubeType CubeType)
{
    ChangeMultiplier(CubeType);

    if (const FDifficulty* DifficultyData = GetDifficultyData();    //
        DifficultyData && DifficultyData->ScoreChangeMap.Contains(CubeType))
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

void ACGGameMode::EnqueueHint(ECubeType CubeType)
{
    EnqueueHint(ConvertCubeTypeToHintType(CubeType));
}

void ACGGameMode::GameOver()
{
    SetPauseAndChangeGameState(EGameState::GameOver);
}

void ACGGameMode::SetPauseAndChangeGameState(EGameState NewGameState)
{
    if (GetWorld() && SetPause(GetWorld()->GetFirstPlayerController()))
    {
        SetGameState(NewGameState);
    }
}

void ACGGameMode::StartPlay()
{
    Super::StartPlay();

    SetupGameMode();

    SetGameState(EGameState::Game);
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ThisClass::OnCountdown, CountdownTimerRate, true);
    GetWorldTimerManager().SetTimer(
        StartupHintDelayTimerHandle,
        [this]()
        {
            EnqueueHint(EHintType::Startup);
        },
        StartupHintDelay, false);
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

void ACGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorldTimerManager().ClearTimer(StartupHintDelayTimerHandle);    // Cos the timer starts lambda.
}

void ACGGameMode::SetupGameMode()
{
    if (!GetWorld())
        return;

    const auto Origin{FTransform::Identity};
    const auto* Field = GetWorld()->SpawnActor<ACGFieldActor>(FieldClass, Origin);
    check(Field);

    const FVector FieldSize = Field->GetSize();
    const auto* BackgroundVFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),                 //
                                                                               BackgroundNiagaraSystem,    //
                                                                               FVector{0.5 * FieldSize.X, 0.5 * FieldSize.Y, VFXSpawnZOffset});
    check(BackgroundVFX);

    if (auto* PlayerPawn = GetWorld()->GetFirstPlayerController() ? GetWorld()->GetFirstPlayerController()->GetPawn<ACGPlayer>() : nullptr)
    {
        PlayerPawn->UpdateLocation(Origin, FieldSize);

        if (auto* BonusComponent = PlayerPawn->FindComponentByClass<UCGBonusComponent>())
        {
            BonusComponent->OnBonusCharged.AddLambda(
                [this](bool IsCharged)
                {
                    EnqueueHint(EHintType::BonusCharged);
                });
        }
    }

    OnMultiplierChanged.AddLambda(
        [this](ECubeType, int32)
        {
            EnqueueHint(EHintType::Multiplier);
        });
    OnLowTime.AddLambda(
        [this]()
        {
            EnqueueHint(EHintType::LowTime);
        });
    OnSpeedChanged.AddLambda(
        [this](int32)
        {
            EnqueueHint(EHintType::SpeedUp);
        });

    FormatHints();

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        CachedHintsStatusMap = GameUserSettings->GetHintsStatus();
        GameUserSettings->OnHintsStatusChanged.AddUObject(this, &ThisClass::OnHintsStatusChanged);
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

EHintType ACGGameMode::ConvertCubeTypeToHintType(ECubeType CubeType)
{
    switch (CubeType)
    {
        case ECubeType::GoodCube:
            return EHintType::GoodCube;
        case ECubeType::BadCube:
            return EHintType::BadCube;
        case ECubeType::ScoreCube:
            return EHintType::ScoreCube;
        case ECubeType::TimeCube:
            return EHintType::TimeCube;
        case ECubeType::BonusCube:
            return EHintType::BonusCube;
        case ECubeType::SpeedCube:
            return EHintType::SpeedCube;
        case ECubeType::VeryBadCube:
            return EHintType::VeryBadCube;
        default:
            return EHintType::Max;
    }
}

void ACGGameMode::InvalidateHintStatus(EHintType HintType)
{
    CachedHintsStatusMap[HintType] = true;
    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SetHintsStatus(CachedHintsStatusMap);
    }
}

void ACGGameMode::FormatHints()
{
    // Format SpeedUp hint.
    if (GameplayHintsMap.Contains(EHintType::SpeedUp) && GameplayHintsMap[EHintType::SpeedUp].HintText.ToString().Contains("{0}"))
    {
        FStringFormatOrderedArguments SpeedUpHintArg;
        if (const FDifficulty* DifficultyData = GetDifficultyData())
        {
            SpeedUpHintArg.Add(GetDifficultyData()->ScoreToSpeedUp);
            GameplayHintsMap[EHintType::SpeedUp].HintText = FText::FromString(FString::Format(*GameplayHintsMap[EHintType::SpeedUp].HintText.ToString(), SpeedUpHintArg));
        }
    }
}

void ACGGameMode::EnqueueHint(EHintType HintType)
{
    if (CachedHintsStatusMap.Contains(HintType) && !CachedHintsStatusMap[HintType])
    {
        HintsQueue.Enqueue(HintType);

        if (!GetWorldTimerManager().IsTimerActive(HintDelayTimerHandle))
        {
            GetWorldTimerManager().SetTimer(HintDelayTimerHandle, this, &ThisClass::OnShowHint, NextHintDelay);
        }
    }
}

void ACGGameMode::OnShowHint()
{
    if (EHintType HintType; HintsQueue.Dequeue(HintType))
    {
        if (CachedHintsStatusMap.Contains(HintType) && !CachedHintsStatusMap[HintType] && GameplayHintsMap.Contains(HintType))
        {
            SetPauseAndChangeGameState(EGameState::PopUpHint);
            OnShowPopUpHint.Broadcast(GameplayHintsMap[HintType]);
            InvalidateHintStatus(HintType);
        }
    }

    if (!HintsQueue.IsEmpty())
    {
        GetWorldTimerManager().SetTimer(HintDelayTimerHandle, this, &ThisClass::OnShowHint, NextHintDelay);
    }
}

void ACGGameMode::OnHintsStatusChanged(const FHintsStatus& NewHintsStatus)
{
    CachedHintsStatusMap = NewHintsStatus;
}

void ACGGameMode::AddTime(int32 TimeToAdd)
{
    GameTime = UKismetMathLibrary::Max(GameTime + TimeToAdd, 0);
    OnTimeChanged.Broadcast(GameTime);

    if (GameTime == 0)
    {
        GameOver();
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
