// Cube_Game, All rights reserved.

#pragma once

#include "CGCoreTypes.generated.h"

using FHintsStatus = TMap<EHintType, bool>;

UENUM(BlueprintType)
enum class EGameState : uint8
{
    WaitingToStart = 0,
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
    Normal = 0,
    Hard,
    Max
};

UENUM(BlueprintType)
enum class ECubeType : uint8
{
    GoodCube = 0,
    BadCube,
    ScoreCube,
    TimeCube,
    BonusCube,
    SpeedCube,
    VeryBadCube,
    None,
    Max
};

UENUM(BlueprintType)
enum class EBonusType : uint8
{
    Missile = 0,
    Shield,
    Bomb,
    None,
    Max
};

UENUM(BlueprintType)
enum class EPopUpType : uint8
{
    Off = 0,
    Multiplier,
    Amount,
    Max
};

UENUM(BlueprintType)
enum class EHintType : uint8
{
    Startup = 0,
    SpeedUp,
    Multiplier,
    LowTime,
    BonusCharged,
    GoodCube,
    BadCube,
    ScoreCube,
    TimeCube,
    BonusCube,
    SpeedCube,
    VeryBadCube,
    Max
};

USTRUCT(BlueprintType)
struct FSpeedData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0", Units = "CentimetersPerSecond"))
    float InitialSpeed{0.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0", Units = "CentimetersPerSecond"))
    float SpeedMultiplier{0.0f};
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
    int32 InitialTime{0};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0", Units = "cm"))
    float DistanceBetweenCubes{0.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty")
    FSpeedData SpeedData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "1"))
    int32 ScoreToSpeedUp{1};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "1"))
    int32 MaxNumOfCubesInLine{1};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ChanceToAddCubeInLine{0.0f};
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
struct FHintSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    bool bHintsEnabled{true};

    // clang-format off
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    TMap<EHintType, bool> HintsStatusMap = {{EHintType::Startup,      false},
                                            {EHintType::Multiplier,   false},
                                            {EHintType::LowTime,      false},
                                            {EHintType::SpeedUp,      false},
                                            {EHintType::BonusCharged, false},
                                            {EHintType::GoodCube,     false},
                                            {EHintType::BadCube,      false},
                                            {EHintType::ScoreCube,    false},
                                            {EHintType::TimeCube,     false},
                                            {EHintType::BonusCube,    false},
                                            {EHintType::SpeedCube,    false},
                                            {EHintType::VeryBadCube,  false}};
    // clang-format on
};

USTRUCT(BlueprintType)
struct FGameSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    EPopUpType PopUpType{EPopUpType::Multiplier};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FHintSettings HintSettings;
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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHintSettingsChangedSignature, const FHintSettings&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBonusBeginOverlapSignature, class ACGCubeActor*, bool);
DECLARE_DELEGATE_OneParam(FOnEndPlaySignature, FDelegateHandle);
