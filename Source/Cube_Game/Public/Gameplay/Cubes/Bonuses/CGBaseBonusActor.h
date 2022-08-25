// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/CGBaseCubeActor.h"
#include "CGBaseBonusActor.generated.h"

class USoundCue;
class UNiagaraComponent;

UCLASS(Abstract)
class CUBE_GAME_API ACGBaseBonusActor : public ACGBaseCubeActor
{
    GENERATED_BODY()

public:
    ACGBaseBonusActor();

    void SetBonusCharged(bool IsCharged) { bCharged = IsCharged; }

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* InitialSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* HitSound;

    bool bCharged = false;

    virtual void BeginPlay() override;

private:
    bool IsCubeNegative(ECubeType CubeType);
};
