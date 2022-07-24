// Cube_Game, All rights reserved.

#include "CGGameModeBase.h"
#include "Player/CGPlayerController.h"

ACGGameModeBase::ACGGameModeBase()
{
    PlayerControllerClass = ACGPlayerController::StaticClass();
}

void ACGGameModeBase::SetGameState(EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;
    OnGameStateChanged.Broadcast(NewGameState);
}

void ACGGameModeBase::OnPressedEnter()
{
    OnPressedEnt.Broadcast();
}

void ACGGameModeBase::OnPressedEscape()
{
    OnPressedEsc.Broadcast();
}
