// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "NiagaraComponent.h"
#include "CGGameMode.h"
#include "Player/CGPlayer.h"

ACGBaseBonusActor::ACGBaseBonusActor()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACGBaseBonusActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    const auto CubeActor = Cast<ACGCubeActor>(OtherActor);
    if (!CubeActor)
        return;

    const auto CubeType = CubeActor->GetCubeType();
    if (bCharged && !IsCubeNegative(CubeType))
    {
        if (const auto Player = GetOwner<ACGPlayer>())
        {
            Player->ReceiveCube(CubeType);
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
    const auto GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr;
    if (!GameMode)
        return true;

    const auto DifficultyVlues = GameMode->GetDifficultyVlues();
    if (DifficultyVlues.ScoreChangeMap.Contains(CubeType))
    {
        if (DifficultyVlues.ScoreChangeMap[CubeType] < 0.0f)
        {
            return true;
        }
    }

    if (DifficultyVlues.TimeChangeMap.Contains(CubeType))
    {
        if (DifficultyVlues.TimeChangeMap[CubeType] < 0.0f)
        {
            return true;
        }
    }

    if (DifficultyVlues.SpeedChangeMap.Contains(CubeType))
    {
        if (DifficultyVlues.SpeedChangeMap[CubeType] > 0.0f)
        {
            return true;
        }
    }

    return false;
}
