// Cube_Game, All rights reserved.

#pragma once

#include "CGCoreTypes.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    WaitingToStart,
    MainMenu,
    Options,
    OptionsWarning,
    DifficultySelection,
    Game,
    PopUpHint,
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
    Max
};

UENUM(BlueprintType)
enum class EPopUpType : uint8
{
    Off,
    Multiplier,
    Amount,
    Max
};

UENUM(BlueprintType)
enum class EHint : uint8
{
    Startup,
    SpeedUp,
    Multiplier,
    LowTime,
    Max
};

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

USTRUCT(BlueprintType)
struct FPopUpHint
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FText Title;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor TitleColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FText HintText;
};

USTRUCT(BlueprintType)
struct FVideoSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0", ClampMax = "4"))
    int32 Quality = 3;
};

USTRUCT(BlueprintType)
struct FSoundSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    float MasterVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    float FXVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    float MusicVolume = 0.5f;
};

USTRUCT(BlueprintType)
struct FHints
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<EHint, bool> HintsMap = {
        TPair<EHint, bool>{EHint::Startup, true},       //
        TPair<EHint, bool>{EHint::Multiplier, true},    //
        TPair<EHint, bool>{EHint::LowTime, true},       //
        TPair<EHint, bool>{EHint::SpeedUp, true}        //
    };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<ECubeType, bool> ReceivingHintsMap = {
        TPair<ECubeType, bool>{ECubeType::GoodCube, true},       //
        TPair<ECubeType, bool>{ECubeType::BadCube, true},        //
        TPair<ECubeType, bool>{ECubeType::ScoreCube, true},      //
        TPair<ECubeType, bool>{ECubeType::TimeCube, true},       //
        TPair<ECubeType, bool>{ECubeType::BonusCube, true},      //
        TPair<ECubeType, bool>{ECubeType::SpeedCube, true},      //
        TPair<ECubeType, bool>{ECubeType::PowerUpCube, true},    //
    };                                                           //
};

USTRUCT(BlueprintType)
struct FGameSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    EPopUpType PopUp = EPopUpType::Multiplier;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FHints Hints;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAspectRatioChangedSignature, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBonusChangedSignature, EBonusType);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPowerupedSignature, EPowerupType);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnScoreChangedSignature, int32, int32, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMultiplierChangedSignature, ECubeType, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpeedChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE(FOnLowTimeSignature);
DECLARE_MULTICAST_DELEGATE(FOnClickedButtonSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowPopUpHintSignature, const FPopUpHint&);
