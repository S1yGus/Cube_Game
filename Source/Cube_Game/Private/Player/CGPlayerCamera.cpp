// Cube_Game, All rights reserved.

#include "Player/CGPlayerCamera.h"
#include "Camera/CameraComponent.h"
#include "CGGameInstance.h"

ACGPlayerCamera::ACGPlayerCamera()
{
    PrimaryActorTick.bCanEverTick = false;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->bConstrainAspectRatio = true;
    SetRootComponent(CameraComponent);
}

void ACGPlayerCamera::BeginPlay()
{
    Super::BeginPlay();

    check(CameraComponent);

    if (const auto GameInstance = GetGameInstance<UCGGameInstance>())
    {
        GameInstance->OnAspectRatioChanged.AddUObject(this, &ACGPlayerCamera::OnAspectRatioChanged);
    }
}

void ACGPlayerCamera::OnAspectRatioChanged(float NewAspectRatio)
{
    CameraComponent->SetAspectRatio(NewAspectRatio);
}
