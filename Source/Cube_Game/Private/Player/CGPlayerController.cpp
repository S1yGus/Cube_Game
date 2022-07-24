// Cube_Game, All rights reserved.

#include "Player/CGPlayerController.h"
#include "CGGameMode.h"
#include "UI/CGHUDBase.h"
#include "Blueprint/UserWidget.h"

void ACGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    check(InputComponent);

    if (const auto GameModeBase = GetGameModeBase())
    {
        InputComponent->BindAction("Enter", EInputEvent::IE_Pressed, GameModeBase, &ACGGameModeBase::OnPressedEnter).bExecuteWhenPaused = true;
        InputComponent->BindAction("Esc", EInputEvent::IE_Pressed, GameModeBase, &ACGGameModeBase::OnPressedEscape).bExecuteWhenPaused = true;
    }
}

void ACGPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (const auto GameModeBase = GetGameModeBase())
    {
        GameModeBase->OnGameStateChanged.AddUObject(this, &ACGPlayerController::OnGameStateChanged);
    }
}

inline ACGGameModeBase* ACGPlayerController::GetGameModeBase() const
{
    return GetWorld()->GetAuthGameMode<ACGGameModeBase>();
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
