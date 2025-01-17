// Cube_Game, All rights reserved.

#include "Player/CGPlayerCamera.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACGPlayerCamera::ACGPlayerCamera()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    check(RootComponent);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    check(SpringArmComponent);
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    check(CameraComponent);
    CameraComponent->bOverrideAspectRatioAxisConstraint = true;
    CameraComponent->AspectRatioAxisConstraint = EAspectRatioAxisConstraint::AspectRatio_MaintainXFOV;
    CameraComponent->SetupAttachment(SpringArmComponent);
}
