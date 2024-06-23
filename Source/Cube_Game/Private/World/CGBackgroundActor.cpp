// Cube_Game, All rights reserved.

#include "World/CGBackgroundActor.h"

void ACGBackgroundActor::BeginPlay()
{
    Super::BeginPlay();

    if (!GetWorld())
        return;

    const double HalfHeight = (RowsNum - 1) * DistanceBetweenBands / 2;
    const double HalfWidth = (ColumnsNum - 1) * DistanceBetweenBands / 2;
    const double ZLocation = -100.0;

    for (int32 i = 0; i < RowsNum; ++i)
    {
        for (int32 j = 0; j < ColumnsNum; ++j)
        {
            const FVector SpawnLocation = FVector{-HalfHeight + (i * DistanceBetweenBands), -HalfWidth + (j * DistanceBetweenBands), ZLocation} + GetActorLocation();
            auto* Band = GetWorld()->SpawnActor(BandActorClass, &SpawnLocation);
            check(Band);
            Bands.Add(Band);
        }
    }
}

void ACGBackgroundActor::OnAudioPlaybackMagnitude(TArray<float> Magnitudes)
{
    if (Magnitudes.IsEmpty())
        return;

    int32 MagnitudeIndex{0};
    for (auto Band : Bands)
    {
        if (IsValid(Band))
        {
            Band->SetActorScale3D(FVector(Magnitudes[MagnitudeIndex] * MagnitudeFactor));
            MagnitudeIndex = (MagnitudeIndex + 1) % Magnitudes.Num();
        }
    }
}
