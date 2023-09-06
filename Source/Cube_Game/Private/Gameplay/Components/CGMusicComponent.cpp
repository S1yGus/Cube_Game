// Cube_Game, All rights reserved.

#include "Gameplay/Components/CGMusicComponent.h"
#include "CGGameMode.h"
#include "Settings/CGGameUserSettings.h"
#include "AudioMixerBlueprintLibrary.h"
#include "Gameplay/CGFieldActor.h"
#include "Gameplay/Cubes/CGBaseCubeActor.h"

constexpr static float MinFrequencyForAnalysis{40.0f};
constexpr static float MaxFrequencyForAnalysis{4000.0f};
constexpr static float IndicatorAmplitudeMultiplier{150.0f};
constexpr static float SpectralAnalysisRate{4.0f};
constexpr static int32 RandHelperRange{9999};

UCGMusicComponent::UCGMusicComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGMusicComponent::BeginPlay()
{
    Super::BeginPlay();

    Setup();
}

void UCGMusicComponent::Setup()
{
    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
    {
        SpeedRange = GameMode->GetSpeedRange();
        GameMode->OnSpeedChanged.AddUObject(this, &ThisClass::OnSpeedChanged);

        OnSubmixSpectralAnalysis.BindDynamic(this, &ThisClass::OnSpectralAnalysis);
        const auto InBandSettings = UAudioMixerBlueprintLibrary::MakeFullSpectrumSpectralAnalysisBandSettings(GameMode->GetMaxMultiplier(),    //
                                                                                                              MinFrequencyForAnalysis,         //
                                                                                                              MaxFrequencyForAnalysis);
        SubmixToAnalysis->AddSpectralAnalysisDelegate(this,                        //
                                                      InBandSettings,              //
                                                      OnSubmixSpectralAnalysis,    //
                                                      SpectralAnalysisRate);
        SubmixToAnalysis->StartSpectralAnalysis(this);
    }

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        OnMusicTypeChanged(GameUserSettings->GetMusicType());
        GameUserSettings->OnMusicTypeChanged.AddUObject(this, &ThisClass::OnMusicTypeChanged);
    }
}

void UCGMusicComponent::UpdateBPM(int32 Speed)
{
    const float CurrentBPM = FMath::GetMappedRangeValueClamped(SpeedRange, BPMRange, static_cast<float>(Speed));
    SetFloatParameter(BPMParamName, CurrentBPM);
}

void UCGMusicComponent::OnSpeedChanged(int32 Speed)
{
    if (bStaticMusic)
        return;

    const int32 BPMSpeedOrder = Speed / SpeedLevelsNumToChangeBPM;
    if (PrevBPMSpeedOrder != BPMSpeedOrder)
    {
        UpdateBPM(Speed);
        PrevBPMSpeedOrder = BPMSpeedOrder;
    }

    const int32 SeedSpeedOrder = Speed / SpeedLevelsNumToChangeSeed;
    if (PrevSeedSpeedOrder != SeedSpeedOrder)
    {
        SetIntParameter(SeedParamName, FMath::RandHelper(RandHelperRange));
        PrevSeedSpeedOrder = SeedSpeedOrder;
    }
}

void UCGMusicComponent::OnMusicTypeChanged(bool bNewMusicType)
{
    bStaticMusic = bNewMusicType;
    SetSound(bStaticMusic ? StaticMusic : DynamicMusic);

    if (!bStaticMusic && GetWorld())
    {
        if (const auto* GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
        {
            UpdateBPM(GameMode->GetGameSpeed());
        }
        SetIntParameter(SeedParamName, FMath::RandHelper(RandHelperRange));
    }
}

void UCGMusicComponent::OnSpectralAnalysis(const TArray<float>& Magnitudes)
{
    if (const auto* Field = GetOwner<ACGFieldActor>())
    {
        for (int32 i = 0; i < Field->Indicators.Num(); ++i)
        {
            if (IsValid(Field->Indicators[i]))
            {
                FVector NewLocation = Field->Indicators[i]->GetActorLocation();
                NewLocation.Z = Magnitudes[i] * IndicatorAmplitudeMultiplier;
                Field->Indicators[i]->SetActorLocation(NewLocation);
            }
        }

        if (IsValid(Field->BonusIndicator))
        {
            FVector NewLocation = Field->BonusIndicator->GetActorLocation();
            NewLocation.Z = Magnitudes[Field->BonusIndicatorPosition] * IndicatorAmplitudeMultiplier;
            Field->BonusIndicator->SetActorLocation(NewLocation);
        }
    }
}
