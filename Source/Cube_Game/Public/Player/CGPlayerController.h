// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CGCoreTypes.h"
#include "CGPlayerController.generated.h"

UCLASS()
class CUBE_GAME_API ACGPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    void OnSetPause();

    virtual void SetupInputComponent() override;
    virtual void BeginPlay() override;

private:
    void OnGameStateChanged(EGameState NewGameState);
};
