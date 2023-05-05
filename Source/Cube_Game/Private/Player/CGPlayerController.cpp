// Cube_Game, All rights reserved.

#include "Player/CGPlayerController.h"
#include "CGGameMode.h"

void ACGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    check(InputComponent);

    InputComponent->BindAction("Enter", EInputEvent::IE_Pressed, this, &ThisClass::OnPressedEnter).bExecuteWhenPaused = true;
    InputComponent->BindAction("Esc", EInputEvent::IE_Pressed, this, &ThisClass::OnPressedEscape).bExecuteWhenPaused = true;
}

void ACGPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (const auto GameModeBase = GetGameModeBase())
    {
        GameModeBase->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }
}

ACGGameModeBase* ACGPlayerController::GetGameModeBase() const
{
    return GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameModeBase>() : nullptr;
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
        SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
        bShowMouseCursor = true;
    }
}

void ACGPlayerController::OnPressedEnter()
{
    OnPressedEnt.Broadcast();
}

void ACGPlayerController::OnPressedEscape()
{
    OnPressedEsc.Broadcast();
}
