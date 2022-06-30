// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGBombBonusActor.h"

ACGBombBonusActor::ACGBombBonusActor()
{
    BonusType = EBonusType::Bomb;
    ScaleInterpSpeed = 70.0f;
    TargetScale = FVector{30.0f};
}

void ACGBombBonusActor::OnScalingDone()
{
    Teardown();
}
