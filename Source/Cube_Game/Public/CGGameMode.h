// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CGGameModeBase.h"
#include "CGCoreTypes.h"
#include "CGGameMode.generated.h"

class ACGMusicActor;
class ACGFieldActor;
class ACGBackgroundActor;
class ACGPlayer;
class UCGBonusComponent;

UCLASS()
class CUBE_GAME_API ACGGameMode : public ACGGameModeBase
{
    GENERATED_BODY()

public:
    ACGGameMode();

    FOnTimeChangedSignature OnTimeChanged;
    FOnLowTimeSignature OnLowTime;
    FOnSpeedChangedSignature OnSpeedChanged;
    FOnScoreChangedSignature OnScoreChanged;
    FOnMultiplierChangedSignature OnMultiplierChanged;
    FOnShowPopUpHintSignature OnShowPopUpHint;

    void UpdateGameMetrics(ECubeType CubeType);
    void EnqueueHint(ECubeType CubeType);

    int32 GetGameTime() const { return GameTime; }
    int32 GetGameSpeed() const { return GameSpeed; }
    int32 GetScore() const { return Score; }
    const FDifficulty& GetDifficultyData() const;

    void GameOver();
    void SetPauseAndChangeGameState(EGameState NewGameState);

    virtual void StartPlay() override;
    virtual bool ClearPause() override;
    virtual void PreInitializeComponents() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficulty, FDifficulty> DifficultyMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time", Meta = (ClampMin = "0", Units = "s"))
    int32 LowTimeThreshold{5};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints", Meta = (ToolTip = "To specify the number of score to increase speed, use the argument {0}."))
    FText SpeedIncreasedHint;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints", Meta = (ClampMin = "0.0", Units = "s"))
    float StartupHintDelay{2.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints", Meta = (ClampMin = "0.0", Units = "s"))
    float NextHintDelay{0.2f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Multiplier", Meta = (ClampMin = "1"))
    int32 MaxMultiplier{8};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    TSubclassOf<ACGMusicActor> MusicActorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    TSubclassOf<ACGFieldActor> FieldActorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    TSubclassOf<ACGBackgroundActor> BackgroundActorClass;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    FTimerHandle CountdownTimerHandle;
    FTimerHandle HintDelayTimerHandle;
    FTimerHandle StartupHintDelayTimerHandle;

    int32 GameTime{0};
    int32 GameSpeed{1};
    int32 Score{0};
    int32 Multiplier{1};

    ECubeType PreviousCubeType{ECubeType::None};

    EDifficulty CurrentDifficulty{EDifficulty::Normal};
    FHintSettings CachedHintSettings;
    TMap<EHintType, FHintData> CachedHintsMap;

    FORCEINLINE ACGPlayer* GetPlayerPawn();
    FORCEINLINE UCGBonusComponent* GetPlayerBonusComponent();

    void SetupGameMode();
    void SetupHints();
    void SpawnActors();
    void UpdatePlayerPawnLocation(const FTransform& Origin, const FVector& FieldSize);
    void OnCountdown();

    FORCEINLINE void InvalidateHintStatus(EHintType HintType);
    void FormatHints();
    void EnqueueHint(EHintType HintType);
    void OnShowHint();
    void OnHintSettingsChanged(const FHintSettings& NewHintSettings);

    FORCEINLINE void AddTime(int32 TimeToAdd);
    FORCEINLINE void AddGameSpeed(int32 SpeedToAdd);
    FORCEINLINE void AddScore(int32 ScoreToAdd);

    void ChangeMultiplier(ECubeType CubeType);
    void ChangeGameTime(ECubeType CubeType);
    void ChangeGameSpeed(ECubeType CubeType);
    void ChangeScore(ECubeType CubeType);
};
