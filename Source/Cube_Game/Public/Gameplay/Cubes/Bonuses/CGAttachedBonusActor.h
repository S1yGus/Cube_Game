// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "CGAttachedBonusActor.generated.h"

UCLASS(Abstract)
class CUBE_GAME_API ACGAttachedBonusActor : public ACGBaseBonusActor
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

private:
    void AttachToPlayerMesh();
};
