// Cube_Game, All rights reserved.

#include "CGGameModeMenu.h"
#include "Player/CGPlayerCamera.h"
#include "UI/CGHUDMenu.h"

constexpr static float CameraRotationTimerRate{0.016f};

ACGGameModeMenu::ACGGameModeMenu()
{
    static ConstructorHelpers::FClassFinder<ACGPlayerCamera> Camera(TEXT("/Game/Player/BP_PlayerCamera"));
    if (Camera.Class)
    {
        DefaultPawnClass = Camera.Class;
    }

    static ConstructorHelpers::FClassFinder<ACGHUDMenu> HUD(TEXT("/Game/Framework/BP_HUDMenu"));
    if (HUD.Class)
    {
        HUDClass = HUD.Class;
    }
}

void ACGGameModeMenu::StartPlay()
{
    Super::StartPlay();

    SetGameState(EGameState::MainMenu);

    FTimerHandle CameraRotationTimerHandle;
    GetWorldTimerManager().SetTimer(CameraRotationTimerHandle, this, &ThisClass::OnCameraRotation, CameraRotationTimerRate, true);
}

void ACGGameModeMenu::OnCameraRotation()
{
    if (GetWorld())
    {
        if (const auto* PC = GetWorld()->GetFirstPlayerController())
        {
            if (auto* Camera = PC->GetPawn())
            {
                Camera->AddActorWorldRotation(FRotator{0.0, CameraRotationSpeed, 0.0});
            }
        }
    }
}
