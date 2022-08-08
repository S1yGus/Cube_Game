// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "NiagaraComponent.h"

ACGBaseBonusActor::ACGBaseBonusActor()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ACGBaseBonusActor::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::PlaySound2D(GetWorld(), InitialSound);

    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACGBaseBonusActor::OnComponentBeginOverlap);
}

void ACGBaseBonusActor::OnOverlapFinished()
{
}

void ACGBaseBonusActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                                AActor* OtherActor,                          //
                                                UPrimitiveComponent* OtherComp,              //
                                                int32 OtherBodyIndex,                        //
                                                bool bFromSweep,                             //
                                                const FHitResult& SweepResult)               //
{
    const auto CubeActor = Cast<ACGCubeActor>(OtherActor);
    if (!CubeActor)
        return;

    UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
    CubeActor->Annihilat();
    OnOverlapFinished();
}
