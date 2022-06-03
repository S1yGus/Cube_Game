// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/Bonuses/CGIndicatorBonusCubeActor.h"
#include "CGBaseBonusActor.generated.h"

class USoundCue;

UCLASS()
class CUBE_GAME_API ACGBaseBonusActor : public ACGIndicatorBonusCubeActor
{
    GENERATED_BODY()

public:
    ACGBaseBonusActor();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* InitialSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* HitSound;

    virtual void BeginPlay() override;
    virtual void OnOverlapFinished();

private:
    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                 AActor* OtherActor,                          //
                                 UPrimitiveComponent* OtherComp,              //
                                 int32 OtherBodyIndex,                        //
                                 bool bFromSweep,                             //
                                 const FHitResult& SweepResult);              //
};
