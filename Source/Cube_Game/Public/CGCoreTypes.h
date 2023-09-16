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
    Leaderboard,
    HowToPlay,
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
    VeryBadCube,
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
enum class EPopUpType : uint8
{
    Off,
    Multiplier,
    Amount,
    Max
};

UENUM(BlueprintType)
enum class EHintType : uint8
{
    Startup,
    SpeedUp,
    Multiplier,
    LowTime,
    BonusCharged,
    Max
};

USTRUCT(BlueprintType)
struct FDifficulty
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, int32> ScoreChangeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, int32> TimeChangeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, int32> SpeedChangeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, float> SpawnWeightMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0", Units = "s"))
    int32 InitialTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0", Units = "cm"))
    float DistanceBetweenCubes;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0", Units = "CentimetersPerSecond"))
    FVector2D CubesSpeedRange;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "1"))
    int32 ScoreToSpeedUp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    int32 MaxNumOfCubesInLine;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0"))
    float ChanceToAddCubeInLine;
};

USTRUCT(BlueprintType)
struct FHintData
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
struct FSoundSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MasterVolume{0.5f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float UIVolume{0.5f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float FXVolume{0.5f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MusicVolume{0.5f};
};

USTRUCT(BlueprintType)
struct FHintsStatus
{
    GENERATED_USTRUCT_BODY()

    // clang-format off
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<EHintType, bool> HintsMap = {
        {EHintType::Startup,      true},
        {EHintType::Multiplier,   true},
        {EHintType::LowTime,      true},
        {EHintType::SpeedUp,      true},
        {EHintType::BonusCharged, true}
    };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<ECubeType, bool> CollectHintsMap = {
        {ECubeType::GoodCube,    true},
        {ECubeType::BadCube,     true},
        {ECubeType::ScoreCube,   true},
        {ECubeType::TimeCube,    true},
        {ECubeType::BonusCube,   true},
        {ECubeType::SpeedCube,   true},
        {ECubeType::VeryBadCube, true}
    };
    // clang-format on
};

USTRUCT(BlueprintType)
struct FGameSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    EPopUpType PopUpType{EPopUpType::Multiplier};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FHintsStatus HintsStatus;
};

USTRUCT(BlueprintType)
struct FPlayerRecord
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerRecord")
    FText Name;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerRecord")
    int32 Score;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerRecord")
    FDateTime DateTime;
};

USTRUCT(BlueprintType)
struct FCultureData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Culture")
    FString Culture;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Culture")
    FText CultureName;
};

USTRUCT(BlueprintType)
struct FCubeColorData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Color")
    FLinearColor Color{FLinearColor::Black};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Color", Meta = (ClampMin = "0.0"))
    float EmissivePower{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Color",
              Meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Can be 1 for enabled condition or 0 for disabled."))
    float MaskEnabled{1.0f};
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBonusChangedSignature, EBonusType);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBonusChargedSignature, bool);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnScoreChangedSignature, int32, int32, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMultiplierChangedSignature, ECubeType, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpeedChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE(FOnLowTimeSignature);
DECLARE_MULTICAST_DELEGATE(FOnClickedButtonSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowPopUpHintSignature, const FHintData&);
DECLARE_MULTICAST_DELEGATE(FOnPressedEscSignature);
DECLARE_MULTICAST_DELEGATE(FOnPressedEnterSignature);
DECLARE_MULTICAST_DELEGATE(FOnResolutionChangedSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPopUpTypeChangedSignature, EPopUpType);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHintsStatusChangedSignature, const FHintsStatus&);
