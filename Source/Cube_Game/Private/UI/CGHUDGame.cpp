// Cube_Game, All rights reserved.

#include "UI/CGHUDGame.h"
#include "CGGameModeBase.h"
#include "UI/CGAnimatedUserWidget.h"

void ACGHUDGame::BackToRootMenu()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameModeBase>();
    if (!GameMode)
        return;

    GameMode->SetGameState(EGameState::Pause);
}

void ACGHUDGame::SetupWidgets()
{
    if (HUDWidgetClass)
    {
        GameWidgets.Add(EGameState::Game, CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass));
    }

    if (PopUpHintWidgetClass)
    {
        GameWidgets.Add(EGameState::PopUpHint, CreateWidget<UUserWidget>(GetWorld(), PopUpHintWidgetClass));
    }

    if (PauseWidgetClass)
    {
        GameWidgets.Add(EGameState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    }

    if (GameOverWidgetClass)
    {
        GameWidgets.Add(EGameState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));
    }

    Super::SetupWidgets();
}
