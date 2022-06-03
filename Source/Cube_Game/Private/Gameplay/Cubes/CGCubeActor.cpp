// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/CGCubeActor.h"
#include "CGGameMode.h"

ACGCubeActor::ACGCubeActor()
{
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ACGCubeActor::Tick(float DeltaSeconds)
{
    Moving(DeltaSeconds);
}

void ACGCubeActor::Moving(float DeltaSeconds)
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode)
        return;

    auto NewLocation = GetActorLocation();
    NewLocation.Y = NewLocation.Y + GameMode->GetCubeSpeed() * DeltaSeconds;
    SetActorLocation(NewLocation);
}
