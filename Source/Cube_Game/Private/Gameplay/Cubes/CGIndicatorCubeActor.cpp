// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/CGIndicatorCubeActor.h"

void ACGIndicatorCubeActor::BeginPlay()
{
    Super::BeginPlay();

    SetMaterial();
}

void ACGIndicatorCubeActor::SetMaterial()
{
    if (!MaterialsMap.Contains(CubeType))
    {
        StaticMeshComponent->SetMaterial(0, DefaultMaterial);
        return;
    }

    StaticMeshComponent->SetMaterial(0, MaterialsMap[CubeType]);
}