// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/Bonuses/CGFollowingBonusActor.h"
#include "CGShieldBonusActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGShieldBonusActor : public ACGFollowingBonusActor
{
    GENERATED_BODY()

public:
    ACGShieldBonusActor();

    void SetDuration(float NewDuration) { ShieldDuration = NewDuration; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield", Meta = (ClampMin = "0.0"))
    float ShieldDuration = 7.0f;

    virtual void BeginPlay() override;

private:
    FTimerHandle ShieldTimerHandle;
};
