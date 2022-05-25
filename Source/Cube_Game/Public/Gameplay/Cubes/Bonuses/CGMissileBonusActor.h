// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "CGMissileBonusActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGMissileBonusActor : public ACGBaseBonusActor
{
    GENERATED_BODY()

public:
    ACGMissileBonusActor();

    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;
    virtual void OnOverlapFinished() override;

private:
    void Moving(float DeltaSeconds);
};
