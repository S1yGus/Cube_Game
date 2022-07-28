// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/Bonuses/CGAttachedBonusActor.h"
#include "CGShieldBonusActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGShieldBonusActor : public ACGAttachedBonusActor
{
    GENERATED_BODY()

public:
    ACGShieldBonusActor();

    void SetDuration(float NewDuration) { ShieldDuration = NewDuration; }

    virtual void Teardown() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield", Meta = (ClampMin = "0.0", Units = "s"))
    float ShieldDuration = 7.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield", Meta = (ClampMin = "0.0"))
    FVector ActivationScale = FVector{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* TeardownSound;

    virtual void BeginPlay() override;
    virtual void OnScalin() override;

private:
    FTimerHandle ShieldTimerHandle;
    bool bCanOffPlayerCollision = true;

    inline void SetPlayerMeshCollision(bool CollisionEnabled);
};
