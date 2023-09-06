// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "CGMissileBonusActor.generated.h"

class UNiagaraComponent;
class UCGMovementComponent;

UCLASS()
class CUBE_GAME_API ACGMissileBonusActor : public ACGBaseBonusActor
{
    GENERATED_BODY()

public:
    ACGMissileBonusActor();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void Teardown() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UNiagaraComponent> NiagaraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCGMovementComponent> MovementComponent;

    virtual void BeginPlay() override;
};
