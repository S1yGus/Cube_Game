// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/CGBaseCubeActor.h"
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

    void SetCubeType(ECubeType NewCubeType) { CubeType = NewCubeType; }
    ECubeType GetCubeType() const { return CubeType; }

    void Annihilate();
    void Collect();

    virtual void SetColor(const FCubeColorData& NewCubeColorData) override;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float LifeDistance{1800.0f};

    virtual void BeginPlay() override;

private:
    ECubeType CubeType{ECubeType::None};

    FORCEINLINE int32 GetCubeSpeed() const;
    FORCEINLINE void EndPlayAction();
    FORCEINLINE void SpawnAnnihilateEffect();
    FORCEINLINE void SpawnCollectEffect();
};
