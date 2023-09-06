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

    void Annihilat();
    void Collect();

    virtual void SetColor(const FCubeColorData& NewCubeColorData) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UNiagaraComponent> TrailNiagaraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCGMovementComponent> MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TObjectPtr<UNiagaraSystem> AnnihilatNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TObjectPtr<UNiagaraSystem> CollectNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraTargetPositionParamName{"TargetPosition"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraVelocityParamName{"Velocity"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraColorParamName{"Color"};

    virtual void BeginPlay() override;

private:
    ECubeType CubeType{ECubeType::None};
    FTimerHandle UpdateNiagaraTargetPositionTimerHandle;
    UPROPERTY()
    TObjectPtr<UNiagaraComponent> CollectNiagaraComponent;

    FORCEINLINE int32 GetCubeSpeed() const;
    void EndPlayAction();
    UNiagaraComponent* SpawnNiagaraEffect(TObjectPtr<UNiagaraSystem> NiagaraSystem);

    void OnUpdateNiagaraTargetPosition();
};
