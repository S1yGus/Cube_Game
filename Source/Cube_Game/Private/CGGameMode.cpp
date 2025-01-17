// Cube_Game, All rights reserved.

#include "CGGameMode.h"
#include "CGGameInstance.h"
#include "Player/CGPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/CGHUDGame.h"
#include "Settings/CGGameUserSettings.h"
#include "Player/Components/CGBonusComponent.h"
#include "World/CGFieldActor.h"
#include "World/CGBackgroundActor.h"
#include "World/CGMusicActor.h"
#include "CGUtils.h"
#include "UObject/ConstructorHelpers.h"

constexpr static float CountdownTimerRate{1.0f};

static TQueue<EHintType> HintsQueue;

ACGGameMode::ACGGameMode()
{
    static ConstructorHelpers::FClassFinder<ACGPlayer> Player(TEXT("/Game/Player/BP_Player"));
    if (Player.Class)
    {
        DefaultPawnClass = Player.Class;
    }

    static ConstructorHelpers::FClassFinder<ACGHUDGame> HUD(TEXT("/Game/Framework/BP_HUDGame"));
    if (HUD.Class)
    {
        HUDClass = HUD.Class;
    }
}

void ACGGameMode::UpdateGameMetrics(ECubeType CubeType)
{
    ChangeMultiplier(CubeType);
    ChangeGameTime(CubeType);
    ChangeGameSpeed(CubeType);
    ChangeScore(CubeType);
}

void ACGGameMode::EnqueueHint(ECubeType CubeType)
{
    EnqueueHint(CubeGame::Utils::CubeTypeToHintType(CubeType));
}

const FDifficulty& ACGGameMode::GetDifficultyData() const
{
    return DifficultyMap[CurrentDifficulty];
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

#if WITH_EDITOR
    const UEnum* DifficultyEnum = StaticEnum<EDifficulty>();
    for (int32 i = 0; i < DifficultyEnum->NumEnums() - 2; ++i)
    {
        check(DifficultyMap.Contains(static_cast<EDifficulty>(i)));
    }
#endif    // WITH_EDITOR

    SetupGameMode();
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

    GameTime = GetDifficultyData().InitialTime;
}

void ACGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorldTimerManager().ClearTimer(StartupHintDelayTimerHandle);    // Cos the timer starts lambda.
}

ACGPlayer* ACGGameMode::GetPlayerPawn()
{
    return GetWorld()->GetFirstPlayerController() ? GetWorld()->GetFirstPlayerController()->GetPawn<ACGPlayer>() : nullptr;
}

UCGBonusComponent* ACGGameMode::GetPlayerBonusComponent()
{
    if (const auto* PlayerPawn = GetPlayerPawn())
    {
        return PlayerPawn->FindComponentByClass<UCGBonusComponent>();
    }
    return nullptr;
}

void ACGGameMode::SetupGameMode()
{
    if (!GetWorld())
        return;

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        CurrentDifficulty = GameUserSettings->GetCurrentDifficulty();
        CachedHintSettings.bHintsEnabled = GameUserSettings->AreHintsEnabled();
        CachedHintSettings.HintsStatusMap = GameUserSettings->GetHintsStatus();
        GameUserSettings->OnHintSettingsChanged.AddUObject(this, &ThisClass::OnHintSettingsChanged);
    }

    if (const auto* GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>())
    {
        CachedHintsMap = GameInstance->GetHintsMap();
    }

    SetupHints();
    SpawnActors();

    SetGameState(EGameState::Game);
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ThisClass::OnCountdown, CountdownTimerRate, true);
}

void ACGGameMode::SetupHints()
{
    FormatHints();

    GetWorldTimerManager().SetTimer(
        StartupHintDelayTimerHandle,
        [this]()
        {
            EnqueueHint(EHintType::Startup);
        },
        StartupHintDelay, false);
    if (auto* BonusComponent = GetPlayerBonusComponent())
    {
        BonusComponent->OnBonusCharged.AddLambda(
            [this](bool IsCharged)
            {
                EnqueueHint(EHintType::BonusCharged);
            });
    }
    OnMultiplierChanged.AddLambda(
        [this](ECubeType, int32)
        {
            EnqueueHint(EHintType::Combo);
        });
    OnLowTime.AddLambda(
        [this]()
        {
            EnqueueHint(EHintType::TimeRunningOut);
        });
    OnSpeedChanged.AddLambda(
        [this](int32)
        {
            EnqueueHint(EHintType::SpeedIncreased);
        });
}

void ACGGameMode::SpawnActors()
{
    // MusicActor
    const auto* MusicActor = GetWorld()->SpawnActor<ACGMusicActor>(MusicActorClass, FTransform::Identity);
    check(MusicActor);

    // FieldActor
    const auto Origin{FTransform::Identity};
    auto* FieldActor = GetWorld()->SpawnActorDeferred<ACGFieldActor>(FieldActorClass, Origin);
    check(FieldActor);
    FieldActor->Init(GetDifficultyData(), GetGameSpeed(), MaxMultiplier);
    OnSpeedChanged.AddUObject(FieldActor, &ACGFieldActor::OnGameSpeedChanged);
    OnMultiplierChanged.AddUObject(FieldActor, &ACGFieldActor::OnMultiplierChanged);
    if (auto* BonusComponent = GetPlayerBonusComponent())
    {
        BonusComponent->OnBonusChanged.AddUObject(FieldActor, &ACGFieldActor::OnBonusChanged);
    }
    FieldActor->FinishSpawning(Origin);

    // BackgroundActor
    const auto FieldSize = FieldActor->GetSize();
    const auto FieldCentre = Origin * FTransform{FVector{FieldSize.X / 2, FieldSize.Y / 2, 0.0}};
    const auto* BackgroundActor = GetWorld()->SpawnActor<ACGBackgroundActor>(BackgroundActorClass, FieldCentre);
    check(BackgroundActor);

    UpdatePlayerPawnLocation(Origin, FieldSize);
}

void ACGGameMode::UpdatePlayerPawnLocation(const FTransform& Origin, const FVector& FieldSize)
{
    if (auto* PlayerPawn = GetPlayerPawn())
    {
        PlayerPawn->UpdateLocation(Origin, FieldSize);
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

void ACGGameMode::InvalidateHintStatus(EHintType HintType)
{
    auto& HintsStatus = CachedHintSettings.HintsStatusMap;
    HintsStatus[HintType] = true;
    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SetHintsStatus(HintsStatus);
    }
}

void ACGGameMode::FormatHints()
{
    // Format SpeedIncreased hint.
    if (CachedHintsMap.Contains(EHintType::SpeedIncreased) && SpeedIncreasedHint.ToString().Contains("{0}"))
    {
        CachedHintsMap[EHintType::SpeedIncreased].HintText = FText::FormatOrdered(SpeedIncreasedHint, LOCGEN_NUMBER_UNGROUPED(GetDifficultyData().ScoreToSpeedUp, ""));
    }
}

void ACGGameMode::EnqueueHint(EHintType HintType)
{
    const auto& HintsStatus = CachedHintSettings.HintsStatusMap;
    if (CachedHintSettings.bHintsEnabled && HintsStatus.Contains(HintType) && !HintsStatus[HintType])
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
        const auto& HintsStatus = CachedHintSettings.HintsStatusMap;
        if (HintsStatus.Contains(HintType) && !HintsStatus[HintType] && CachedHintsMap.Contains(HintType))
        {
            SetPauseAndChangeGameState(EGameState::PopUpHint);
            OnShowPopUpHint.Broadcast(CachedHintsMap[HintType]);
            InvalidateHintStatus(HintType);
        }
    }

    if (!HintsQueue.IsEmpty())
    {
        GetWorldTimerManager().SetTimer(HintDelayTimerHandle, this, &ThisClass::OnShowHint, NextHintDelay);
    }
}

void ACGGameMode::OnHintSettingsChanged(const FHintSettings& NewHintSettings)
{
    CachedHintSettings = NewHintSettings;
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
    if (CubeType == PreviousCubeType && CubeGame::Utils::IsCubeAffectsMetrics(CubeType, DifficultyMap[CurrentDifficulty]))
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

void ACGGameMode::ChangeGameTime(ECubeType CubeType)
{
    if (const auto& DifficultyData = GetDifficultyData();    //
        DifficultyData.TimeChangeMap.Contains(CubeType))
    {
        AddTime(Multiplier * DifficultyData.TimeChangeMap[CubeType]);
    }
}

void ACGGameMode::ChangeGameSpeed(ECubeType CubeType)
{
    if (const auto& DifficultyData = GetDifficultyData();    //
        DifficultyData.SpeedChangeMap.Contains(CubeType))
    {
        AddGameSpeed(Multiplier * DifficultyData.SpeedChangeMap[CubeType]);
    }
}

void ACGGameMode::ChangeScore(ECubeType CubeType)
{
    if (const auto& DifficultyData = GetDifficultyData();    //
        DifficultyData.ScoreChangeMap.Contains(CubeType))
    {
        const int32 ScoreRemains = Score % DifficultyData.ScoreToSpeedUp;
        const int32 ScoreToAdd = Multiplier * DifficultyData.ScoreChangeMap[CubeType];
        if (ScoreToAdd > 0)
        {
            if (const int32 SpeedToAdd = (ScoreRemains + ScoreToAdd) / DifficultyData.ScoreToSpeedUp)
            {
                AddGameSpeed(SpeedToAdd);
            }
        }

        AddScore(ScoreToAdd);
    }
}
