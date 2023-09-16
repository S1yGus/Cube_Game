// Cube_Game, All rights reserved.

#include "Gameplay/Components/CGMusicComponent.h"
#include "AudioMixerBlueprintLibrary.h"

constexpr static float SpectralAnalysisRate{4.0f};

UCGMusicComponent::UCGMusicComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGMusicComponent::BeginPlay()
{
    Super::BeginPlay();

    const auto InBandSettings = UAudioMixerBlueprintLibrary::MakeFullSpectrumSpectralAnalysisBandSettings(NumBands,                   //
                                                                                                          MinFrequencyForAnalysis,    //
                                                                                                          MaxFrequencyForAnalysis);
    SubmixToAnalysis->AddSpectralAnalysisDelegate(this,                        //
                                                  InBandSettings,              //
                                                  OnSubmixSpectralAnalysis,    //
                                                  SpectralAnalysisRate);
    SubmixToAnalysis->StartSpectralAnalysis(this);
}
