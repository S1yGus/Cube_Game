// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "CGMissileBonusActor.generated.h"

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
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UNiagaraComponent* NiagaraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCGMovementComponent* MovementComponent;

    virtual void BeginPlay() override;
};
