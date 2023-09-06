// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "CGGameMode.h"
#include "Player/CGPlayer.h"

ACGBaseBonusActor::ACGBaseBonusActor()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACGBaseBonusActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    auto* CubeActor = Cast<ACGCubeActor>(OtherActor);
    if (!CubeActor)
        return;

    const ECubeType CubeType = CubeActor->GetCubeType();
    if (bCharged && !IsCubeNegative(CubeType))
    {
        if (auto* Player = GetOwner<ACGPlayer>())
        {
            Player->CollectCube(CubeType);
            CubeActor->Teardown();
        }
    }
    else
    {
        UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
        CubeActor->Annihilat();
    }
}

void ACGBaseBonusActor::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::PlaySound2D(GetWorld(), InitialSound);
}

bool ACGBaseBonusActor::IsCubeNegative(ECubeType CubeType)
{
    if (const auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
    {
        if (const FDifficulty* DifficultyData = GameMode->GetDifficultyData())
        {
            if (DifficultyData->ScoreChangeMap.Contains(CubeType))
            {
                if (DifficultyData->ScoreChangeMap[CubeType] < 0.0f)
                {
                    return true;
                }
            }

            if (DifficultyData->TimeChangeMap.Contains(CubeType))
            {
                if (DifficultyData->TimeChangeMap[CubeType] < 0.0f)
                {
                    return true;
                }
            }

            if (DifficultyData->SpeedChangeMap.Contains(CubeType))
            {
                if (DifficultyData->SpeedChangeMap[CubeType] > 0.0f)
                {
                    return true;
                }
            }
        }
    }

    return false;
}
