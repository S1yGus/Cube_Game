// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/CGIndicatorCubeActor.h"
#include "CGCoreTypes.h"
#include "CGCubeActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGCubeActor : public ACGIndicatorCubeActor
{
    GENERATED_BODY()

public:
    ACGCubeActor();

    virtual void Tick(float DeltaSeconds) override;

    ECubeType GetCubeType() const { return CubeType; }

private:
    void Moving(float DeltaSeconds);
};
