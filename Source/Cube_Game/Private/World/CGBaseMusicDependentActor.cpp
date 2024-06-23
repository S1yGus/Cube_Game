// Cube_Game, All rights reserved.

#include "World/CGBaseMusicDependentActor.h"
#include "World/CGMusicActor.h"
#include "Kismet/GameplayStatics.h"

ACGBaseMusicDependentActor::ACGBaseMusicDependentActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ACGBaseMusicDependentActor::BeginPlay()
{
    Super::BeginPlay();

    BindAudioPlaybackMagnitudeHandler();
}

void ACGBaseMusicDependentActor::BindAudioPlaybackMagnitudeHandler()
{
    auto* MusicActor = Cast<ACGMusicActor>(UGameplayStatics::GetActorOfClass(this, ACGMusicActor::StaticClass()));
    check(MusicActor);
    MusicActor->OnAudioPlaybackMagnitude.AddUObject(this, &ThisClass::OnAudioPlaybackMagnitude);
}

void ACGBaseMusicDependentActor::OnAudioPlaybackMagnitude(TArray<float> Magnitudes)
{
}
