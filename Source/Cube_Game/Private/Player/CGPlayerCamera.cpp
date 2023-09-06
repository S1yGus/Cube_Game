// Cube_Game, All rights reserved.

#include "Player/CGPlayerCamera.h"
#include "Camera/CameraComponent.h"
#include "Settings/CGGameUserSettings.h"

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

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->OnAspectRatioChanged.AddUObject(this, &ThisClass::OnAspectRatioChanged);
        OnAspectRatioChanged(GameUserSettings->GetAspectRatio());    // Aspect ratio initialization.
    }
}

void ACGPlayerCamera::OnAspectRatioChanged(const FAspectRatioData& NewAspectRatioData)
{
    CameraComponent->SetAspectRatio(NewAspectRatioData.AspectRatio);
    CameraComponent->SetFieldOfView(NewAspectRatioData.FOV);
}
