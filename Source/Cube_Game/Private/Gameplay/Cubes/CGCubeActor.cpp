// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/CGCubeActor.h"
#include "Gameplay/Cubes/Components/CGMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CGGameMode.h"
#include "CGUtils.h"

ACGCubeActor::ACGCubeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    TrailNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Trail");
    check(TrailNiagaraComponent);
    TrailNiagaraComponent->SetupAttachment(StaticMeshComponent);

    MovementComponent = CreateDefaultSubobject<UCGMovementComponent>("MovementComponent");
    check(MovementComponent);

    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACGCubeActor::Annihilate()
{
    EndPlayAction();
    SpawnAnnihilateEffect();
}

void ACGCubeActor::Collect()
{
    EndPlayAction();
    SpawnCollectEffect();
}

void ACGCubeActor::SetColor(const FCubeColorData& NewCubeColorData)
{
    Super::SetColor(NewCubeColorData);

    TrailNiagaraComponent->SetColorParameter(NiagaraColorParamName, NewCubeColorData.Color);
}

void ACGCubeActor::BeginPlay()
{
    Super::BeginPlay();

    check(TrailNiagaraComponent->GetAsset());

    SetLifeSpan(LifeDistance / GetCubeSpeed());
}

int32 ACGCubeActor::GetCubeSpeed() const
{
    if (GetWorld())
    {
        if (const auto* GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
        {
            return GameMode->GetCubeSpeed();
        }
    }
    return 0;
}

void ACGCubeActor::EndPlayAction()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetVisibility(false, true);
    MovementComponent->StopMoving();
}

void ACGCubeActor::SpawnAnnihilateEffect()
{
    UNiagaraComponent* NiagaraFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, AnnihilateNiagaraSystem, GetActorLocation());
    check(NiagaraFX);
    NiagaraFX->SetColorParameter(NiagaraColorParamName, CubeColorData.Color * CubeColorData.EmissivePower);
    NiagaraFX->SetVectorParameter(NiagaraVelocityParamName, FVector{-static_cast<double>(MovementComponent->GetCubeSpeed()), 0.0, 0.0});
    NiagaraFX->SetVectorParameter(NiagaraCubeSizeParamName, CubeGame::Utils::GetMeshAABBBoxSize(StaticMeshComponent->GetStaticMesh()));
}

void ACGCubeActor::SpawnCollectEffect()
{
    if (auto* PlayerMesh = GetPlayerMesh())
    {
        UNiagaraComponent* NiagaraFX = UNiagaraFunctionLibrary::SpawnSystemAttached(CollectNiagaraSystem,             //
                                                                                    PlayerMesh,                       //
                                                                                    CollectEffectSocketName,          //
                                                                                    FVector::ZeroVector,              //
                                                                                    FRotator::ZeroRotator,            //
                                                                                    EAttachLocation::SnapToTarget,    //
                                                                                    true);
        check(NiagaraFX);
        NiagaraFX->SetColorParameter(NiagaraColorParamName, CubeColorData.Color * CubeColorData.EmissivePower);
        NiagaraFX->SetVariablePosition(NiagaraSpawnPositionParamName, GetActorLocation());
        NiagaraFX->SetVectorParameter(NiagaraCubeSizeParamName, CubeGame::Utils::GetMeshAABBBoxSize(StaticMeshComponent->GetStaticMesh()));
    }
}
