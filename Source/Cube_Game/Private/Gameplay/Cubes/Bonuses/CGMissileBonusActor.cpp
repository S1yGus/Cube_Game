// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGMissileBonusActor.h"

constexpr static float MissileSpeed = 1000.0f;
constexpr static float LifeSpan = 3.0f;

ACGMissileBonusActor::ACGMissileBonusActor()
{
    PrimaryActorTick.bCanEverTick = true;

    BonusType = EBonusType::Missile;
    TargetScale = FVector{0.2};
}

void ACGMissileBonusActor::Tick(float DeltaSeconds)
{
    Moving(DeltaSeconds);
}

void ACGMissileBonusActor::BeginPlay()
{
    Super::BeginPlay();

    SetLifeSpan(LifeSpan);
}

void ACGMissileBonusActor::OnOverlapFinished()
{
    Teardown();
}

void ACGMissileBonusActor::Moving(float DeltaSeconds)
{
    auto NewLocation = GetActorLocation();
    NewLocation.Y = NewLocation.Y - MissileSpeed * DeltaSeconds;
    SetActorLocation(NewLocation);
}
