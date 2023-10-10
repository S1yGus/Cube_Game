// Cube_Game, All rights reserved.

#include "Player/CGPlayerCamera.h"
#include "Camera/CameraComponent.h"

ACGPlayerCamera::ACGPlayerCamera()
{
    PrimaryActorTick.bCanEverTick = false;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    check(CameraComponent);
    CameraComponent->bOverrideAspectRatioAxisConstraint = true;
    CameraComponent->AspectRatioAxisConstraint = EAspectRatioAxisConstraint::AspectRatio_MaintainXFOV;
    SetRootComponent(CameraComponent);
}
