// Cube_Game, All rights reserved.

#include "UI/CGHUDMenu.h"
#include "CGGameModeBase.h"
#include "Blueprint/UserWidget.h"

void ACGHUDMenu::BeginPlay()
{
    Super::BeginPlay();

    check(MainMenuWidgetClass);
    check(FirstLaunchOptionsWidgetClass);
    check(DifficultySelectionWidgetClass);
    check(LeaderboardWidgetClass);
}

void ACGHUDMenu::BackToRootMenu()
{
    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameModeBase>() : nullptr)
    {
        GameMode->SetGameState(EGameState::MainMenu);
    }
}

void ACGHUDMenu::SetupWidgets()
{
    GameWidgets.Add(EGameState::MainMenu, CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass));
    GameWidgets.Add(EGameState::FirstLaunchOptions, CreateWidget<UUserWidget>(GetWorld(), FirstLaunchOptionsWidgetClass));
    GameWidgets.Add(EGameState::DifficultySelection, CreateWidget<UUserWidget>(GetWorld(), DifficultySelectionWidgetClass));
    GameWidgets.Add(EGameState::Leaderboard, CreateWidget<UUserWidget>(GetWorld(), LeaderboardWidgetClass));

    Super::SetupWidgets();
}