// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "CGFollowingBonusActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGFollowingBonusActor : public ACGBaseBonusActor
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle FollowTimerHandle;

    void OnFollowing();
};
