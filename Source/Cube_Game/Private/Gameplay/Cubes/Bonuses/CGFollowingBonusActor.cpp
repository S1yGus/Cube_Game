// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGFollowingBonusActor.h"

constexpr static float FollowingTimerRate = 0.016f;

void ACGFollowingBonusActor::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(FollowTimerHandle, this, &ACGFollowingBonusActor::OnFollowing, FollowingTimerRate, true);
}

void ACGFollowingBonusActor::OnFollowing()
{
    const auto PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
        return;

    const auto PlayerPawn = PlayerController->GetPawn();
    if (!PlayerPawn)
        return;

    const auto StaticMesh = PlayerPawn->FindComponentByClass<UStaticMeshComponent>();
    if (!StaticMesh)
        return;

    SetActorLocation(StaticMesh->GetComponentLocation());
}
