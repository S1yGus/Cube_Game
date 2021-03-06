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
    FOnShowPopUpHintSignature OnShowPopUpHintSignature;

    int32 GetTime() const { return Time; }
    int32 GetSpeed() const { return Speed; }
    int32 GetCubeSpeed() const;
    int32 GetScore() const { return Score; }
    const FDifficulty* GetDifficultyVlues() const;
    const TMap<ECubeType, FPopUpHint>& GetReceivingHints() const { return ReceivingHintsMap; }

    void ChangeTime(ECubeType CubeType);
    void ChangeSpeed(ECubeType CubeType);
    void ChangeScore(ECubeType CubeType);

    void ShowPopUpHint(const FPopUpHint& PopUpHint);

    void GameOver();

    virtual void StartPlay() override;
    virtual bool ClearPause() override;
    virtual void PreInitializeComponents() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficulty, FDifficulty> DifficultyMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    FVector2D SpeedRange{1.0f, 999.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time", Meta = (ClampMin = "0"))
    int32 LowTimeThreshold = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<EHint, FPopUpHint> GameplayHintsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<ECubeType, FPopUpHint> ReceivingHintsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints", Meta = (ClampMin = "0"))
    float StartupHintDelay = 2.0f;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    FTimerHandle CountdownTimerHandle;
    FTimerHandle DelayHintTimerHandle;

    int32 Time = 0;
    int32 Speed = 1;
    int32 Score = 0;
    int32 Multiplier = 1;

    ECubeType PreviousCubeType = ECubeType::None;

    void SetupGameMode();
    void OnCountdown();

    void ShowGameplayHint(EHint Hint, float Delay = 0.0f);
    void FormatHints();
    void OnShowMultiplierHint(ECubeType CubeType, int32 CurrentMultiplier);
    void OnShowLowTimeHint();
    void OnShowSpeedUpHint(int32 NewSpeed);

    void AddTime(int32 TimeToAdd);
    void AddSpeed(int32 SpeedToAdd);
    void AddScore(int32 ScoreToAdd);
    void ChangeMultiplier(ECubeType CubeType);
};
