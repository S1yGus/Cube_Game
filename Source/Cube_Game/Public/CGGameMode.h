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

    virtual void StartPlay() override;
    virtual bool ClearPause() override;
    virtual void PreInitializeComponents() override;

    void ChangeTime(ECubeType CubeType);
    void ChangeSpeed(ECubeType CubeType);
    void ChangeScore(ECubeType CubeType);

    int32 GetTime() const { return Time; }
    int32 GetSpeed() const { return Speed; }
    int32 GetCubeSpeed() const;
    int32 GetScore() const { return Score; }
    const FDifficulty* GetDifficultyVlues() const;

    void ShowPopUpHint(const FPopUpHint& PopUpHint);
    const TMap<ECubeType, FPopUpHint>& GetReceivingHints() const { return ReceivingHintsMap; }

    void GameOver();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficulty, FDifficulty> DifficultyMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time", Meta = (ClampMin = "0"))
    int32 LowTimeThreshold = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<EHint, FPopUpHint> PopUpHintsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<ECubeType, FPopUpHint> ReceivingHintsMap;

private:
    FTimerHandle CountdownTimerHandle;
    FTimerHandle StartupHintTimerHandle;

    int32 Time = 0;
    int32 Speed = 1;
    int32 Score = 0;
    int32 Multiplier = 1;

    ECubeType PreviousCubeType = ECubeType::None;

    void SetupGameMode();
    void OnCountdown();
    void OnShowStartupHint();

    void AddTime(int32 TimeToAdd);
    void AddSpeed(int32 SpeedToAdd);
    void AddScore(int32 ScoreToAdd);
    void ChangeMultiplier(ECubeType CubeType);
};
