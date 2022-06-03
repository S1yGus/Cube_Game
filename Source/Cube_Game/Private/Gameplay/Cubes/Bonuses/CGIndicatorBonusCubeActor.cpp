// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGIndicatorBonusCubeActor.h"

void ACGIndicatorBonusCubeActor::BeginPlay()
{
    Super::BeginPlay();

    SetMaterial();
}

void ACGIndicatorBonusCubeActor::SetMaterial()
{
    if (!MaterialsMap.Contains(BonusType))
    {
        StaticMeshComponent->SetMaterial(0, DefaultMaterial);
        return;
    }

    StaticMeshComponent->SetMaterial(0, MaterialsMap[BonusType]);
}