// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGBombBonusActor.h"

ACGBombBonusActor::ACGBombBonusActor()
{
    BonusType = EBonusType::Bomb;
    ScaleInterpSpeed = 5000.0f;
    TargetScale = FVector{50.0f};
}

void ACGBombBonusActor::OnScalingDone()
{
    Teardown();
}
