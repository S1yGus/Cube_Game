// Cube_Game, All rights reserved.

#include "CGGameModeBase.h"
#include "Player/CGPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ACGGameModeBase::ACGGameModeBase()
{
    static ConstructorHelpers::FClassFinder<ACGPlayerController> PC(TEXT("/Game/Player/BP_PlayerController"));
    if (PC.Class)
    {
        PlayerControllerClass = PC.Class;
    }
}

void ACGGameModeBase::SetGameState(EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;
    OnGameStateChanged.Broadcast(NewGameState);
}
