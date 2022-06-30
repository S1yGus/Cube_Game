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

    ECubeType GetCubeType() const { return CubeType; }

    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

private:
    inline int32 GetCubeSpeed() const;
    void Moving(float DeltaSeconds);
};
