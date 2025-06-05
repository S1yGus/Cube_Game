// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CGGameModeBase.h"
#include "CGGameModeCredits.generated.h"

UCLASS()
class CUBE_GAME_API ACGGameModeCredits : public ACGGameModeBase
{
    GENERATED_BODY()

public:
    ACGGameModeCredits();

    virtual void StartPlay() override;

private:
    void OnPressedEscape();
};
