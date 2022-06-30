// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/Bonuses/CGAttachedBonusActor.h"
#include "CGBombBonusActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGBombBonusActor : public ACGAttachedBonusActor
{
    GENERATED_BODY()

public:
    ACGBombBonusActor();

protected:
    virtual void OnScalingDone() override;
};
