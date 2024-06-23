// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/Cubes/Bonuses/CGAttachedBonusActor.h"
#include "CGShieldBonusActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGShieldBonusActor : public ACGAttachedBonusActor
{
    GENERATED_BODY()

public:
    virtual void Teardown() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield", Meta = (ClampMin = "0.0", Units = "s"))
    float ShieldDuration{5.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield", Meta = (ClampMin = "0.0", Units = "s"))
    float ChargedShieldDuration{7.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield", Meta = (ClampMin = "0.0"))
    FVector ActivationScale{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> TeardownSound;

    virtual void BeginPlay() override;
    virtual void OnScaling() override;

private:
    FTimerHandle ShieldTimerHandle;

    FORCEINLINE void SetPlayerMeshCollisionEnabled(bool bEnabled);
};
