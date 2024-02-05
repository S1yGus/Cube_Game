// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/CGBaseCubeActor.h"
#include "CGBaseBonusActor.generated.h"

class USoundCue;

UCLASS(Abstract)
class CUBE_GAME_API ACGBaseBonusActor : public ACGBaseCubeActor
{
    GENERATED_BODY()

public:
    ACGBaseBonusActor();

    FOnBonusBeginOverlapSignature OnBonusBeginOverlap;

    void SetBonusCharged(bool IsCharged) { bCharged = IsCharged; }

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> InitialSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> HitSound;

    bool bCharged{false};

    virtual void BeginPlay() override;
};
