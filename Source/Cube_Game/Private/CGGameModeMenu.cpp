// Cube_Game, All rights reserved.

#include "CGGameModeMenu.h"
#include "Player/CGPlayerCamera.h"
#include "UI/CGHUDMenu.h"

ACGGameModeMenu::ACGGameModeMenu()
{
    DefaultPawnClass = ACGPlayerCamera::StaticClass();
    HUDClass = ACGHUDMenu::StaticClass();
}

void ACGGameModeMenu::StartPlay()
{
    Super::StartPlay();

    SetGameState(EGameState::MainMenu);
}
