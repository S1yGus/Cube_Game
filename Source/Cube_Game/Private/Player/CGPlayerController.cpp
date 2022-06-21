// Cube_Game, All rights reserved.

#include "Player/CGPlayerController.h"
#include "CGGameMode.h"

void ACGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &ACGPlayerController::OnSetPause);
}

void ACGPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameModeBase>())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ACGPlayerController::OnGameStateChanged);
    }
}

void ACGPlayerController::OnSetPause()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode)
        return;

    if (GameMode->IsPaused())
    {
        GameMode->ClearPause();
    }
    else
    {
        GameMode->SetPause(this);
        GameMode->SetGameState(EGameState::Pause);
    }
}

void ACGPlayerController::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState == EGameState::Game)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}