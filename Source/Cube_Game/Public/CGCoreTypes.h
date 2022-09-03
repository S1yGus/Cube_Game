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
    TMap<ECubeType, float> ScoreChangeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, float> TimeChangeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, float> SpeedChangeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    TMap<ECubeType, float> SpawnWeightMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0", Units = "s"))
    int32 InitialTime = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0", Units = "cm"))
    float DistanceBetweenCubes = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0", Units = "cm/s"))
    FVector2D CubesSpeedRange{100.0f, 10000.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "1"))
    int32 ScoreToSpeedUp = 1000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    int32 MaxNumOfCubesInLine = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0"))
    float ChanceToAddCubeInLine = 0.4;
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
struct FAspectRatioData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    float AspectRatio = 1.7777777f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    float FOV = 90.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FText DisplayName{FText::FromString("16:9")};
};

USTRUCT(BlueprintType)
struct FVideoSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FAspectRatioData AspectRatioData;
};

USTRUCT(BlueprintType)
struct FSoundSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MasterVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float UIVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float FXVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MusicVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    bool bStaticMusic = true;
};

USTRUCT(BlueprintType)
struct FHintsStatus
{
    GENERATED_USTRUCT_BODY()

    // clang-format off
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<EHintType, bool> HintsMap = {
        TPair<EHintType, bool>{EHintType::Startup,      true},
        TPair<EHintType, bool>{EHintType::Multiplier,   true},
        TPair<EHintType, bool>{EHintType::LowTime,      true},
        TPair<EHintType, bool>{EHintType::SpeedUp,      true},
        TPair<EHintType, bool>{EHintType::BonusCharged, true}
    };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<ECubeType, bool> ReceivingHintsMap = {
        TPair<ECubeType, bool>{ECubeType::GoodCube,    true},
        TPair<ECubeType, bool>{ECubeType::BadCube,     true},
        TPair<ECubeType, bool>{ECubeType::ScoreCube,   true},
        TPair<ECubeType, bool>{ECubeType::TimeCube,    true},
        TPair<ECubeType, bool>{ECubeType::BonusCube,   true},
        TPair<ECubeType, bool>{ECubeType::SpeedCube,   true},
        TPair<ECubeType, bool>{ECubeType::VeryBadCube, true}
    };
    // clang-format on
};

USTRUCT(BlueprintType)
struct FGameSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    EPopUpType PopUpType = EPopUpType::Multiplier;

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
    FLinearColor Color = FLinearColor::Black;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Color", Meta = (ClampMin = "0.0"))
    float EmissivePower = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Color",
              Meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Can be 1 for enabled condition or 0 for disabled."))
    float MaskEnabled = 1.0f;
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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAspectRatioChangedSignature, const FAspectRatioData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPopUpTypeChangedSignature, EPopUpType);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHintsStatusChangedSignature, const FHintsStatus&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMusicTypeChangedSignature, bool);
