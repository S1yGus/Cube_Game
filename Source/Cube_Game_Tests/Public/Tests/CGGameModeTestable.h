// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CGGameMode.h"
#include "CGGameModeTestable.generated.h"

UCLASS()
class CUBE_GAME_TESTS_API ACGGameModeTestable : public ACGGameMode
{
    GENERATED_BODY()

public:
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
};
