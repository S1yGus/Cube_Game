// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGCoreTypes.h"
#include "CGFieldActor.generated.h"

class ACGGameMode;
class ACGCubeActor;
class ACGIndicatorCubeActor;
class ACGIndicatorBonusCubeActor;
class UWidgetComponent;

UCLASS()
class CUBE_GAME_API ACGFieldActor : public AActor
{
    GENERATED_BODY()

public:
    ACGFieldActor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* WidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
    FName ColorParamName = "Color";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
    FLinearColor DefaultColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
    TMap<EBonusType, FLinearColor> MaterialColorsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<ACGCubeActor> SpawningCubeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", Meta = (ClampMin = "0"))
    int32 SpawnPositionsAmount = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", Meta = (ClampMin = "0.0", Units = "cm"))
    float SpawnStep = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn | Offset", Meta = (Units = "cm"))
    float SpawnXOffset = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn | Offset", Meta = (Units = "cm"))
    float SpawnYOffset = -1600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn | Offset", Meta = (Units = "cm"))
    float SpawnZOffset = 65.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Indicators")
    TSubclassOf<ACGIndicatorCubeActor> IndicatorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Indicators")
    TSubclassOf<ACGIndicatorBonusCubeActor> BonusIndicatorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators", Meta = (ClampMin = "0.0", Units = "cm"))
    float IndicatorsSpawnStep = 207.15f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators | Offset", Meta = (Units = "cm"))
    float IndicatorsSpawnYOffset = -100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators | Offset", Meta = (Units = "cm"))
    float IndicatorsSpawnZOffset = 65.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators", Meta = (ClampMin = "0"))
    int32 BonusIndicatorPosition = 7;

    virtual void BeginPlay() override;

private:
    FTimerHandle SpawnTimerHandle;
    bool bRestartSpawn = false;

    int32 CubesInLine = 0;
    TArray<int32> SpawnPositions;

    UPROPERTY()
    TArray<ACGIndicatorCubeActor*> Indicators;
    UPROPERTY()
    ACGIndicatorBonusCubeActor* BonusIndicator;
    EBonusType NewBonusIndicatorType = EBonusType::None;

    float GetSpawnTimerRate() const;
    ECubeType GetRandomCubeType() const;
    inline ACGGameMode* GetGameMode() const;
    inline const FDifficulty* GetDifficultyVlues() const;
    inline const APawn* GetPlayerPawn() const;

    void SetupField();

    void OnSpawnCube();
    void SpawnCube(int32 SpawnPosition);
    void OnSpeedChanged(int32 NewSpeed);
    void RestoreSpawnPositions();

    void OnMultiplierChanged(ECubeType CubeType, int32 Multiplier);
    void SpawnIndicator(ECubeType CubeType, int32 Multiplier);

    void OnBonusChanged(EBonusType BonusType);
    void ChangeFieldColor(EBonusType BonusType);
    void SpawnBonusIndicator(EBonusType BonusType);
    UFUNCTION()
    void OnBonusIndicatorDestroyed(AActor* DestroyedActor);
};
