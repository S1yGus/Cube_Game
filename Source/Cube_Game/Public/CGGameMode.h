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
    const FVector2D& GetSpeedRange() const { return SpeedRange; }
    int32 GetCubeSpeed() const;
    int32 GetScore() const { return Score; }
    const FDifficulty* GetDifficultyData() const;
    const TMap<ECubeType, FHintData>& GetCollectHints() const { return CollectHintsMap; }
    int32 GetMaxMultiplier() const { return MaxMultiplier; }

    void ChangeGameTime(ECubeType CubeType);
    void ChangeGameSpeed(ECubeType CubeType);
    void ChangeScore(ECubeType CubeType);

    void ShowPopUpHint(const FHintData& HintData);

    void GameOver();

    virtual void StartPlay() override;
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause());
    bool SetPause(EGameState NewGameState);
    virtual bool ClearPause() override;
    virtual void PreInitializeComponents() override;
    virtual void SetGameState(EGameState NewGameState) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficulty, FDifficulty> DifficultyMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    FVector2D SpeedRange{1.0f, 999.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time", Meta = (ClampMin = "0", Units = "s"))
    int32 LowTimeThreshold{5};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<EHintType, FHintData> GameplayHintsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<ECubeType, FHintData> CollectHintsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints", Meta = (ClampMin = "0", Units = "s"))
    float StartupHintDelay{2.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Multiplier", Meta = (ClampMin = "1"))
    int32 MaxMultiplier{8};

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    FTimerHandle CountdownTimerHandle;
    FTimerHandle DelayHintTimerHandle;

    int32 GameTime{0};
    int32 GameSpeed{1};
    int32 Score{0};
    int32 Multiplier{1};

    ECubeType PreviousCubeType{ECubeType::None};

    TMap<EHintType, bool> CachedHintsStatusMap;

    bool bGameOver{false};
    bool bShowingHint{false};

    void SetupGameMode();
    void OnCountdown();

    void ShowGameplayHint(EHintType HintType, float Delay = 0.0f);
    void FormatHints();

    void OnShowMultiplierHint(ECubeType CubeType, int32 CurrentMultiplier);
    void OnShowLowTimeHint();
    void OnShowSpeedUpHint(int32 NewSpeed);
    void OnShowBonusChargedHint(bool IsCharged);
    FORCEINLINE void OnHintsStatusChanged(const FHintsStatus& NewHintsStatus);

    FORCEINLINE void AddTime(int32 TimeToAdd);
    FORCEINLINE void AddGameSpeed(int32 SpeedToAdd);
    FORCEINLINE void AddScore(int32 ScoreToAdd);
    void ChangeMultiplier(ECubeType CubeType);

    FORCEINLINE void CheckFlags(EGameState NewGameState);
};
