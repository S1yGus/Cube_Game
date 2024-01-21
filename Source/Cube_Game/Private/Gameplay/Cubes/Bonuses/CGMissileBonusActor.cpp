// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGMissileBonusActor.h"
#include "Gameplay/Cubes/Components/CGMovementComponent.h"
#include "NiagaraComponent.h"
#include "Gameplay/Cubes/CGCubeActor.h"

constexpr static float MissileLifeSpan{4.0f};

ACGMissileBonusActor::ACGMissileBonusActor()
{
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Trail");
    NiagaraComponent->SetupAttachment(StaticMeshComponent);

    MovementComponent = CreateDefaultSubobject<UCGMovementComponent>("MovementComponent");
}

void ACGMissileBonusActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (Cast<ACGCubeActor>(OtherActor) && !bCharged)
    {
        Teardown();
    }
}

void ACGMissileBonusActor::Teardown()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetVisibility(false);
    NiagaraComponent->Deactivate();
    MovementComponent->StopMoving();
}

void ACGMissileBonusActor::BeginPlay()
{
    Super::BeginPlay();

    check(NiagaraComponent);
    check(MovementComponent);

    SetLifeSpan(MissileLifeSpan);
}
