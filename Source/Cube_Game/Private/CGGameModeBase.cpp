// Cube_Game, All rights reserved.

#include "CGGameModeBase.h"

void ACGGameModeBase::SetGameState(EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;
    OnGameStateChanged.Broadcast(NewGameState);
}
