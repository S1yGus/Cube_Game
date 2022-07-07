// Cube_Game, All rights reserved.

#include "UI/CGHUDMenu.h"
#include "CGGameModeBase.h"
#include "UI/CGAnimatedUserWidget.h"

void ACGHUDMenu::BackToRootMenu()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameModeBase>();
    if (!GameMode)
        return;

    GameMode->SetGameState(EGameState::MainMenu);
}

void ACGHUDMenu::SetupWidgets()
{
    if (MainMenuWidgetClass)
    {
        GameWidgets.Add(EGameState::MainMenu, CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass));
    }

    if (DifficultySelectionWidgetClass)
    {
        GameWidgets.Add(EGameState::DifficultySelection, CreateWidget<UUserWidget>(GetWorld(), DifficultySelectionWidgetClass));
    }

    if (LeaderboardWidgetClass)
    {
        GameWidgets.Add(EGameState::Leaderboard, CreateWidget<UUserWidget>(GetWorld(), LeaderboardWidgetClass));
    }

    Super::SetupWidgets();
}