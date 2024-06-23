// Cube_Game, All rights reserved.

#include "World/Cubes/Bonuses/CGAttachedBonusActor.h"

void ACGAttachedBonusActor::BeginPlay()
{
    Super::BeginPlay();

    AttachToPlayerMesh();
}

void ACGAttachedBonusActor::AttachToPlayerMesh()
{
    if (UStaticMeshComponent* PlayerMesh = GetPlayerMesh())
    {
        AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }
}
