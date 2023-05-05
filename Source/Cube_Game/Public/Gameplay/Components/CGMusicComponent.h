// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "CGMusicComponent.generated.h"

class USoundCue;
class USoundSubmix;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBE_GAME_API UCGMusicComponent : public UAudioComponent
{
    GENERATED_BODY()

public:
    UCGMusicComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MetaSounds|BPM")
    FName BPMParamName = "BPM";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MetaSounds|BPM")
    FVector2D BPMRange{220.0f, 400.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MetaSounds|BPM")
    int32 SpeedLevelsNumToChangeBPM = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MetaSounds|Seed")
    FName SeedParamName = "Seed";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MetaSounds|Seed")
    int32 SpeedLevelsNumToChangeSeed = 30;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* StaticMusic;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundBase* DynamicMusic;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundSubmix* SubmixToAnalysis;

    virtual void BeginPlay() override;

private:
    FOnSubmixSpectralAnalysisBP OnSubmixSpectralAnalysis;

    bool bStaticMusic = false;
    int32 PrevBPMSpeedOrder = 0;
    int32 PrevSeedSpeedOrder = 0;
    FVector2D SpeedRange{1.0f, 999.0f};

    void Setup();
    void UpdateBPM(int32 Speed);

    void OnSpeedChanged(int32 Speed);
    void OnMusicTypeChanged(bool bNewMusicType);
    UFUNCTION()
    void OnSpectralAnalysis(const TArray<float>& Magnitudes);
};
