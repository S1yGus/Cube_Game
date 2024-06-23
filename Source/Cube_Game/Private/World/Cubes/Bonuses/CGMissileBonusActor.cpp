// Cube_Game, All rights reserved.

#include "World/Cubes/Bonuses/CGMissileBonusActor.h"
#include "World/Cubes/Components/CGMovementComponent.h"
#include "World/Cubes/CGCubeActor.h"
#include "NiagaraComponent.h"

constexpr static float MissileLifeSpan{4.0f};

ACGMissileBonusActor::ACGMissileBonusActor()
{
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Trail");
    check(NiagaraComponent);
    NiagaraComponent->SetupAttachment(StaticMeshComponent);

    MovementComponent = CreateDefaultSubobject<UCGMovementComponent>("MovementComponent");
    check(MovementComponent);
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

    check(NiagaraComponent->GetAsset());

    SetLifeSpan(MissileLifeSpan);
}
