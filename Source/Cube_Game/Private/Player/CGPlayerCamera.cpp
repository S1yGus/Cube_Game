// Cube_Game, All rights reserved.

#include "Player/CGPlayerCamera.h"
#include "Camera/CameraComponent.h"

ACGPlayerCamera::ACGPlayerCamera()
{
    PrimaryActorTick.bCanEverTick = false;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    check(CameraComponent);
    CameraComponent->bConstrainAspectRatio = true;
    SetRootComponent(CameraComponent);
}
