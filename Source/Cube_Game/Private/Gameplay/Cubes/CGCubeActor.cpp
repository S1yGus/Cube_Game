// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/CGCubeActor.h"
#include "CGGameMode.h"

constexpr static int32 LifeDistance = 2500;

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

void ACGCubeActor::BeginPlay()
{
    Super::BeginPlay();

    SetLifeSpan(LifeDistance / GetCubeSpeed());
}

int32 ACGCubeActor::GetCubeSpeed() const
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    return GameMode ? GameMode->GetCubeSpeed() : 0;
}

void ACGCubeActor::Moving(float DeltaSeconds)
{
    auto NewLocation = GetActorLocation();
    NewLocation.Y = NewLocation.Y + GetCubeSpeed() * DeltaSeconds;
    SetActorLocation(NewLocation);
}
