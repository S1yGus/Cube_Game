// Cube_Game, All rights reserved.

#include "Gameplay/Components/CGMetaSoundMusicComponent.h"
#include "CGGameMode.h"
#include "Sound/SoundCue.h"
#include "Settings/CGGameUserSettings.h"

constexpr static int32 RandHelperRange = 9999;

UCGMetaSoundMusicComponent::UCGMetaSoundMusicComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGMetaSoundMusicComponent::BeginPlay()
{
    Super::BeginPlay();

    Setup();
}

void UCGMetaSoundMusicComponent::Setup()
{
    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
    {
        SpeedRange = GameMode->GetSpeedRange();
        GameMode->OnSpeedChanged.AddUObject(this, &ThisClass::OnSpeedChanged);
    }

    if (const auto GameUserSettings = UCGGameUserSettings::Get())
    {
        OnMusicTypeChanged(GameUserSettings->GetMusicType());
        GameUserSettings->OnMusicTypeChanged.AddUObject(this, &ThisClass::OnMusicTypeChanged);
    }
}

void UCGMetaSoundMusicComponent::UpdateBPM(int32 Speed)
{
    const auto CurrentBPM = FMath::GetMappedRangeValueClamped(SpeedRange, BPMRange, static_cast<float>(Speed));
    SetFloatParameter(BPMParamName, CurrentBPM);
}

void UCGMetaSoundMusicComponent::OnSpeedChanged(int32 Speed)
{
    if (bStaticMusic)
        return;

    const auto BPMSpeedOrder = Speed / SpeedLevelsNumToChangeBPM;
    if (PrevBPMSpeedOrder != BPMSpeedOrder)
    {
        UpdateBPM(Speed);
        PrevBPMSpeedOrder = BPMSpeedOrder;
    }

    const auto SeedSpeedOrder = Speed / SpeedLevelsNumToChangeSeed;
    if (PrevSeedSpeedOrder != SeedSpeedOrder)
    {
        SetIntParameter(SeedParamName, FMath::RandHelper(RandHelperRange));
        PrevSeedSpeedOrder = SeedSpeedOrder;
    }
}

void UCGMetaSoundMusicComponent::OnMusicTypeChanged(bool NewMusicType)
{
    bStaticMusic = NewMusicType;
    SetSound(bStaticMusic ? StaticMusic : DynamicMusic);

    if (!bStaticMusic)
    {
        if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
        {
            UpdateBPM(GameMode->GetSpeed());
        }
        SetIntParameter(SeedParamName, FMath::RandHelper(RandHelperRange));
    }
}
