// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/CGBaseMusicDependentActor.h"
#include "CGBackgroundActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGBackgroundActor : public ACGBaseMusicDependentActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    TSubclassOf<AActor> BandActorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0"))
    int32 RowsNum{8};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0"))
    int32 ColumnsNum{12};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0"))
    float DistanceBetweenBands{250.0f};

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TArray<TObjectPtr<AActor>> Bands;

    virtual void OnAudioPlaybackMagnitude(TArray<float> Magnitudes) override;
};
