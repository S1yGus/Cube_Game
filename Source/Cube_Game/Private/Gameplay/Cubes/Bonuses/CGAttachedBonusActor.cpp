// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGAttachedBonusActor.h"

void ACGAttachedBonusActor::BeginPlay()
{
    Super::BeginPlay();

    AttachToPlayerMesh();
}

UStaticMeshComponent* ACGAttachedBonusActor::GetPlayerMesh() const
{
    const auto PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
        return nullptr;

    const auto PlayerPawn = PlayerController->GetPawn();
    if (!PlayerPawn)
        return nullptr;

    return PlayerPawn->FindComponentByClass<UStaticMeshComponent>();
}

void ACGAttachedBonusActor::AttachToPlayerMesh()
{
    const auto PlayerMesh = GetPlayerMesh();
    if (!PlayerMesh)
        return;

    AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
