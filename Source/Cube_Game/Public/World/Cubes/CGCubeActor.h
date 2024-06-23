// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/Cubes/CGBaseCubeActor.h"
#include "CGCoreTypes.h"
#include "CGCubeActor.generated.h"

class UCGMovementComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class CUBE_GAME_API ACGCubeActor : public ACGBaseCubeActor
{
    GENERATED_BODY()

public:
    ACGCubeActor();

    FOnEndPlaySignature OnEndPlay;

    void Init(const FSpeedData& InSpeedData, int32 InGameSpeed, ECubeType InCubeType, const FCubeColorData& InCubeColorData, float InLifeDistance,
              const FDelegateHandle& InOnSpeedChangedHandle);
    ECubeType GetCubeType() const { return CubeType; }

    void Annihilate();
    void Collect();

    void OnSpeedChanged(int32 InGameSpeed);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UNiagaraComponent> TrailNiagaraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCGMovementComponent> MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TObjectPtr<UNiagaraSystem> AnnihilateNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TObjectPtr<UNiagaraSystem> CollectNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName CollectEffectSocketName{"CentralSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraCubeSizeParamName{"Size"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraSpawnPositionParamName{"SpawnPosition"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraVelocityParamName{"Velocity"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraColorParamName{"Color"};

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    FTimerHandle TeardownTimerHandle;
    ECubeType CubeType{ECubeType::None};
    float LifeDistance{0.0f};
    FDelegateHandle OnSpeedChangedHandle;

    FORCEINLINE void SetTeardownTimer();
    FORCEINLINE void EndPlayAction();
    FORCEINLINE void SpawnAnnihilateEffect();
    FORCEINLINE void SpawnCollectEffect();
};
