// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGCoreTypes.h"
#include "CGFieldActor.generated.h"

class ACGGameMode;
class ACGCubeActor;
class ACGBaseCubeActor;
class UWidgetComponent;
class UCGMusicComponent;

UCLASS()
class CUBE_GAME_API ACGFieldActor : public AActor
{
    GENERATED_BODY()

public:
    ACGFieldActor();

    void Init(const FDifficulty& DifficultyData, int32 GameSpeed, int32 MaxMultiplier);
    FVector GetSize() const;

    void OnGameSpeedChanged(int32 InSpeed);
    void OnMultiplierChanged(ECubeType CubeType, int32 Multiplier);
    void OnBonusChanged(EBonusType BonusType);

    virtual void PostInitializeComponents() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UWidgetComponent> WidgetComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UCGMusicComponent> MusicComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials|Field", Meta = (ToolTip = "Use none type for the default color."))
    TMap<EBonusType, FLinearColor> FieldMaterialColorsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials|Field")
    FName FieldColorParamName{"EmissiveColor"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials|Cubes")
    TMap<ECubeType, FCubeColorData> CubeColorDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials|Cubes")
    TMap<EBonusType, FCubeColorData> BonusColorDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<ACGCubeActor> SpawningCubeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", Meta = (ClampMin = "0"))
    int32 SpawnPositionsAmount{4};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", Meta = (ClampMin = "0.0", Units = "cm"))
    float CubesSpawnStep{200.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Offset", Meta = (Units = "cm"))
    float CubesSpawnXOffset{1600.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Offset", Meta = (Units = "cm"))
    float CubesSpawnYOffset{100.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Offset", Meta = (Units = "cm"))
    float CubesSpawnZOffset{65.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Indicators")
    TSubclassOf<ACGBaseCubeActor> IndicatorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators", Meta = (ClampMin = "0.0", Units = "cm"))
    float IndicatorsSpawnStep{207.15f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators|Offset", Meta = (Units = "cm"))
    float IndicatorsSpawnXOffset{100.0f};

    virtual void BeginPlay() override;

private:
    FOnSpeedChangedSignature OnSpeedChanged;

    FDifficulty CurrentDifficultyData;
    int32 CurrentGameSpeed{0};

    FTimerHandle SpawnTimerHandle;
    bool bRestartSpawnAfterSpeedChanged{false};
    int32 CubesInLine{0};
    TArray<int32> SpawnPositions;

    UPROPERTY()
    TArray<TObjectPtr<ACGBaseCubeActor>> Indicators;
    UPROPERTY()
    TObjectPtr<ACGBaseCubeActor> BonusIndicator;
    EBonusType NewBonusIndicatorType{EBonusType::None};
    int32 BonusIndicatorPosition{0};

    FORCEINLINE float GetSpawnTimerRate() const;
    FORCEINLINE ECubeType GetRandomCubeType() const;

    void OnSpawnCube();
    void SpawnCube(int32 SpawnPosition);
    void RestoreSpawnPositions();
    void SpawnIndicator(ECubeType CubeType, int32 Multiplier);

    void ChangeFieldColor(EBonusType BonusType);
    void SpawnBonusIndicator(EBonusType BonusType);
    UFUNCTION()
    void OnBonusIndicatorDestroyed(AActor* DestroyedActor);
    UFUNCTION()
    void OnSpectralAnalysis(const TArray<float>& Magnitudes);

    template <class T>
    T* SpawnCubeActor(UClass* CubeClass, const FVector& RelativeLocation)
    {
        const auto SpawnTransform = FTransform{RelativeLocation} * GetTransform();
        T* SpawnedCube = GetWorld() ? GetWorld()->SpawnActor<T>(CubeClass, SpawnTransform) : nullptr;
        check(SpawnedCube);
        return SpawnedCube;
    }
};
