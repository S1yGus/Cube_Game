// Cube_Game, All rights reserved.

#include "UI/CGHUDGame.h"
#include "CGGameModeBase.h"
#include "Blueprint/UserWidget.h"

void ACGHUDGame::BeginPlay()
{
    Super::BeginPlay();

    check(HUDWidgetClass);
    check(PopUpHintWidgetClass);
    check(PauseWidgetClass);
    check(GameOverWidgetClass);
}

void ACGHUDGame::BackToRootMenu()
{
    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameModeBase>() : nullptr)
    {
        GameMode->SetGameState(EGameState::Pause);
    }
}

void ACGHUDGame::SetupWidgets()
{
    GameWidgets.Add(EGameState::Game, CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass));
    GameWidgets.Add(EGameState::PopUpHint, CreateWidget<UUserWidget>(GetWorld(), PopUpHintWidgetClass));
    GameWidgets.Add(EGameState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(EGameState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    Super::SetupWidgets();
}
