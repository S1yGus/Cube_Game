// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/CGBaseCubeActor.h"
#include "CGBaseBonusActor.generated.h"

class USoundCue;
class UNiagaraComponent;

UCLASS()
class CUBE_GAME_API ACGBaseBonusActor : public ACGBaseCubeActor
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
