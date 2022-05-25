// Cube_Game, All rights reserved.

#include "Player/CGPlayerController.h"
#include "CGGameMode.h"

void ACGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &ACGPlayerController::OnSetPause);
}

void ACGPlayerController::OnSetPause()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode)
        return;

    GameMode->IsPaused() ? GameMode->ClearPause() : GameMode->SetPause(this);
}
