// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "CGMusicComponent.generated.h"

class USoundSubmix;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBE_GAME_API UCGMusicComponent : public UAudioComponent
{
    GENERATED_BODY()

public:
    UCGMusicComponent();

    FOnSubmixSpectralAnalysisBP OnSubmixSpectralAnalysis;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Analysis")
    TObjectPtr<USoundSubmix> SubmixToAnalysis;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Analysis")
    int32 NumBands{8};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Analysis")
    float MinFrequencyForAnalysis{40.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Analysis")
    float MaxFrequencyForAnalysis{4000.0f};

    virtual void BeginPlay() override;
};
