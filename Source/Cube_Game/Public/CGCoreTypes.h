// Cube_Game, All rights reserved.

#pragma once

#include "CGCoreTypes.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    WaitingToStart,
    InMainMenu,
    InOptions,
    InGame,
    Pause,
    GameOver,
    Max
};

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
    Easy,
    Medium,
    Hard,
    Max
};

UENUM(BlueprintType)
enum class ECubeType : uint8
{
    None,
    GoodCube,
    BadCube,
    ScoreCube,
    TimeCube,
    BonusCube,
    SpeedCube,
    PowerUpCube,
    Max
};

UENUM(BlueprintType)
enum class EBonusType : uint8
{
    None,
    Missile,
    Shield,
    Bomb,
    Max
};

UENUM(BlueprintType)
enum class EPowerupType : uint8
{
    Uber,
    Speed,
    Max
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAspectRatioChangedSignature, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBonusChangedSignature, EBonusType);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPowerupedSignature, EPowerupType);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnScoreChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMultiplierChangedSignature, ECubeType, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpeedChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE(FOnLowTimeSignature);

USTRUCT(BlueprintType)
struct FDifficulty
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, float> ScoreChangeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, float> TimeChangeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, float> SpeedChangeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, float> SpawnWeightMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0"))
    int32 InitialTime = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0"))
    float DistanceBetweenCubes = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    FVector2D CubesSpeedRange{100.0f, 10000.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "1"))
    int32 ScoreToSpeedUp = 1000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    int32 MaxNumOfCubesInLine = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0"))
    float ChanceToAddCubeInLine = 0.4;
};
