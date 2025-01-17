// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGBaseMusicDependentActor.generated.h"

UCLASS(Abstract)
class CUBE_GAME_API ACGBaseMusicDependentActor : public AActor
{
    GENERATED_BODY()

public:
    ACGBaseMusicDependentActor();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Analyzer", Meta = (ClampMin = "0.0"))
    FVector MagnitudeFactor{1.0f};

    virtual void BeginPlay() override;

private:
    void BindAudioPlaybackMagnitudeHandler();

    virtual void OnAudioPlaybackMagnitude(TArray<float> Magnitudes);
};
