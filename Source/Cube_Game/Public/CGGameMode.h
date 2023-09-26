// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CGGameModeBase.h"
#include "CGCoreTypes.h"
#include "CGGameMode.generated.h"

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

    int32 GetGameTime() const { return GameTime; }
    int32 GetGameSpeed() const { return GameSpeed; }
    int32 GetCubeSpeed() const;
    int32 GetScore() const { return Score; }
    const FDifficulty* GetDifficultyData() const;
    int32 GetMaxMultiplier() const { return MaxMultiplier; }

    void ChangeGameTime(ECubeType CubeType);
    void ChangeGameSpeed(ECubeType CubeType);
    void ChangeScore(ECubeType CubeType);
    void EnqueueHint(ECubeType CubeType);

    void GameOver();
    void SetPauseAndChangeGameState(EGameState NewGameState);

    virtual void StartPlay() override;
    virtual bool ClearPause() override;
    virtual void PreInitializeComponents() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficulty, FDifficulty> DifficultyMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    FVector2D SpeedRange{1.0f, 999.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time", Meta = (ClampMin = "0", Units = "s"))
    int32 LowTimeThreshold{5};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<EHintType, FHintData> GameplayHintsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints", Meta = (ClampMin = "0", Units = "s"))
    float StartupHintDelay{2.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints", Meta = (ClampMin = "0", Units = "s"))
    float NextHintDelay{0.2f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Multiplier", Meta = (ClampMin = "1"))
    int32 MaxMultiplier{8};

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

    FHintsStatus CachedHintsStatusMap;

    void SetupGameMode();
    void OnCountdown();

    FORCEINLINE EHintType ConvertCubeTypeToHintType(ECubeType CubeType);
    FORCEINLINE void InvalidateHintStatus(EHintType HintType);
    void FormatHints();
    void EnqueueHint(EHintType HintType);
    void OnShowHint();
    void OnHintsStatusChanged(const FHintsStatus& NewHintsStatus);

    FORCEINLINE void AddTime(int32 TimeToAdd);
    FORCEINLINE void AddGameSpeed(int32 SpeedToAdd);
    FORCEINLINE void AddScore(int32 ScoreToAdd);
    void ChangeMultiplier(ECubeType CubeType);
};
