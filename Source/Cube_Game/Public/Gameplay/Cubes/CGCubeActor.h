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

    virtual void SetColor(const FCubeColorData& NewCubeColorData) override;
    virtual void Teardown() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNiagaraComponent* NiagaraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCGMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* AnnihilationNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* ReceivingNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraTargetPositionParamName = "TargetPosition";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraVelocityParamName = "Velocity";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName NiagaraColorParamName = "Color";

    virtual void BeginPlay() override;

private:
    ECubeType CubeType = ECubeType::None;
    FTimerHandle UpdatePositionTimerHandle;
    UPROPERTY()
    UNiagaraComponent* ReceivingNiagaraComponent;

    inline int32 GetCubeSpeed() const;
    void EndPlayAction();
    UNiagaraComponent* SpawnEndPlayNiagaraEffect(UNiagaraSystem* NiagaraSystem);

    void OnUpdatePosition();
};
