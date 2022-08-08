// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGAttachedBonusActor.h"

void ACGAttachedBonusActor::BeginPlay()
{
    Super::BeginPlay();

    AttachToPlayerMesh();
}

void ACGAttachedBonusActor::AttachToPlayerMesh()
{
    const auto PlayerMesh = GetPlayerMesh();
    if (!PlayerMesh)
        return;

    AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
