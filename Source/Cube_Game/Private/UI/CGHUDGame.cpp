// Cube_Game, All rights reserved.

#include "UI/CGHUDGame.h"
#include "CGGameModeBase.h"
#include "UI/CGAnimatedUserWidget.h"

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
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameModeBase>();
    if (!GameMode)
        return;

    GameMode->SetGameState(EGameState::Pause);
}

void ACGHUDGame::SetupWidgets()
{
    GameWidgets.Add(EGameState::Game, CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass));
    GameWidgets.Add(EGameState::PopUpHint, CreateWidget<UUserWidget>(GetWorld(), PopUpHintWidgetClass));
    GameWidgets.Add(EGameState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(EGameState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    Super::SetupWidgets();
}
