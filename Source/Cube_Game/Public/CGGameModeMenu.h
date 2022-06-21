// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CGGameModeBase.h"
#include "CGGameModeMenu.generated.h"

UCLASS()
class CUBE_GAME_API ACGGameModeMenu : public ACGGameModeBase
{
    GENERATED_BODY()

public:
    ACGGameModeMenu();

    virtual void StartPlay() override;
};
