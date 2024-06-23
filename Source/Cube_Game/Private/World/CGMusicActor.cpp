// Cube_Game, All rights reserved.

#include "World/CGMusicActor.h"
#include "Components/AudioComponent.h"
#include "ConstantQNRT.h"

ACGMusicActor::ACGMusicActor()
{
    PrimaryActorTick.bCanEverTick = false;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
    check(AudioComponent);

    UnusedIndexes.Reserve(Analyzers.Num());
}

void ACGMusicActor::BeginPlay()
{
    Super::BeginPlay();

    check(!Analyzers.IsEmpty());

    AudioComponent->OnAudioFinished.AddDynamic(this, &ThisClass::InitAudio);
    AudioComponent->OnAudioPlaybackPercent.AddDynamic(this, &ThisClass::OnAudioPlaybackPercent);
    InitAudio();
}

void ACGMusicActor::RestoreUnusedIndexes()
{
    for (int8 i = 0; i < Analyzers.Num(); ++i)
    {
        UnusedIndexes.Add(i);
    }
}

void ACGMusicActor::InitAudio()
{
    if (UnusedIndexes.IsEmpty())
    {
        RestoreUnusedIndexes();
    }
    const int8 NewIndex = UnusedIndexes[FMath::RandHelper(UnusedIndexes.Num())];
    CurrentAnalyzer = Analyzers[NewIndex];
    UnusedIndexes.RemoveSingle(NewIndex);

    AudioComponent->SetSound(CurrentAnalyzer->Sound);
    AudioComponent->Play();
}

void ACGMusicActor::OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
    const float Duration = PlayingSoundWave->GetDuration() * PlaybackPercent;
    TArray<float> Magnitudes;
    CurrentAnalyzer->GetNormalizedChannelConstantQAtTime(Duration, 0, Magnitudes);

    OnAudioPlaybackMagnitude.Broadcast(Magnitudes);
}
