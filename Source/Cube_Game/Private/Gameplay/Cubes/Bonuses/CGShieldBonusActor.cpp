// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGShieldBonusActor.h"

ACGShieldBonusActor::ACGShieldBonusActor()
{
    BonusType = EBonusType::Shield;
    ScaleInterpSpeed = 3.0f;
    TargetScale = FVector{1.3f};
}

void ACGShieldBonusActor::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(ShieldTimerHandle, this, &ACGShieldBonusActor::Teardown, ShieldDuration);
}
