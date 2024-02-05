// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Cubes/CGCubeActor.h"

ACGBaseBonusActor::ACGBaseBonusActor()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACGBaseBonusActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if (auto* Cube = Cast<ACGCubeActor>(OtherActor))
    {
        UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
        OnBonusBeginOverlap.Broadcast(Cube, bCharged);
    }
}

void ACGBaseBonusActor::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::PlaySound2D(GetWorld(), InitialSound);
}
