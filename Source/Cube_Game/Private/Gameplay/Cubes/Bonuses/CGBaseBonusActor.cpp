// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "CGGameMode.h"
#include "Player/CGPlayer.h"

ACGBaseBonusActor::ACGBaseBonusActor()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACGBaseBonusActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    auto* CubeActor = Cast<ACGCubeActor>(OtherActor);
    if (!CubeActor || !GetWorld())
        return;

    const auto* GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode)
        return;

    if (const ECubeType CubeType = CubeActor->GetCubeType(); bCharged && !GameMode->IsCubeNegative(CubeType))
    {
        if (auto* Player = GetOwner<ACGPlayer>())
        {
            Player->CollectCube(CubeType);
            CubeActor->Collect();
        }
    }
    else
    {
        UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
        CubeActor->Annihilate();
    }
}

void ACGBaseBonusActor::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::PlaySound2D(GetWorld(), InitialSound);
}
