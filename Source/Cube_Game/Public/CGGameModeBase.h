// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGCoreTypes.h"
#include "CGGameModeBase.generated.h"

UCLASS()
class CUBE_GAME_API ACGGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    FOnGameStateChangedSignature OnGameStateChanged;

    ACGGameModeBase();

    virtual void SetGameState(EGameState NewGameState);

private:
    EGameState CurrentGameState = EGameState::WaitingToStart;
};
