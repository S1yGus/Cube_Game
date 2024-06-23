// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "CGCoreTypes.h"
#include "CGMusicActor.generated.h"

class UAudioComponent;
class UConstantQNRT;

UCLASS()
class CUBE_GAME_API ACGMusicActor : public AInfo
{
    GENERATED_BODY()

public:
    ACGMusicActor();

    FOnAudioPlaybackMagnitudeSignature OnAudioPlaybackMagnitude;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UAudioComponent> AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Analyzer")
    TArray<TObjectPtr<UConstantQNRT>> Analyzers;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<UConstantQNRT> CurrentAnalyzer;
    TArray<int8> UnusedIndexes;

    void RestoreUnusedIndexes();
    UFUNCTION()
    void InitAudio();
    UFUNCTION()
    void OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent);
};
