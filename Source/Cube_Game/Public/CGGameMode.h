// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CGGameModeBase.h"
#include "CGCoreTypes.h"
#include "CGGameMode.generated.h"

class USoundCue;

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

    virtual void StartPlay() override;
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    void ChangeTime(ECubeType CubeType);
    void ChangeSpeed(ECubeType CubeType);
    void ChangeSpeed(int32 NewSpeed);
    void ChangeScore(ECubeType CubeType);

    int32 GetSpeed() const { return Speed; }
    int32 GetCubeSpeed() const;
    int32 GetScore() const { return Score; }
    const FDifficulty* GetDifficultyVlues() const;

    void GameOver();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<EDifficulty, FDifficulty> DifficultyMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time", Meta = (ClampMin = "0"))
    int32 LowTimeThreshold = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* LowTimeSound;

private:
    FTimerHandle CountdownTimerHandle;

    int32 Time = 0;
    int32 Speed = 1;
    int32 Score = 0;
    int32 Multiplier = 1;

    ECubeType PreviousCubeType = ECubeType::None;

    void SetupGameMode();
    void OnCountdown();

    void SetTime(int32 NewTime);
    void SetSpeed(int32 NewSpeed);
    void SetScore(int32 NewScore);
    void ChangeMultiplier(ECubeType CubeType);
};
