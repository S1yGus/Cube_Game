// Cube_Game, All rights reserved.

#include "World/Cubes/CGCubeActor.h"
#include "World/Cubes/Components/CGMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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

void ACGCubeActor::Init(const FSpeedData& InSpeedData, int32 InGameSpeed, ECubeType InCubeType, const FCubeColorData& InCubeColorData, float InLifeDistance,
                        const FDelegateHandle& InOnSpeedChangedHandle)
{
    MovementComponent->SetSpeedData(InSpeedData);
    MovementComponent->SetGameSpeed(InGameSpeed);
    CubeType = InCubeType;
    LifeDistance = InLifeDistance;
    OnSpeedChangedHandle = InOnSpeedChangedHandle;
    TrailNiagaraComponent->SetColorParameter(NiagaraColorParamName, InCubeColorData.Color);
    SetColor(InCubeColorData);
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

void ACGCubeActor::OnSpeedChanged(int32 InGameSpeed)
{
    MovementComponent->SetGameSpeed(InGameSpeed);
    SetTeardownTimer();
}

void ACGCubeActor::BeginPlay()
{
    Super::BeginPlay();

    check(TrailNiagaraComponent->GetAsset());

    SetTeardownTimer();
}

void ACGCubeActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (OnEndPlay.IsBound())
    {
        OnEndPlay.Execute(OnSpeedChangedHandle);
    }
}

void ACGCubeActor::SetTeardownTimer()
{
    const float CubeSpeed = MovementComponent->GetSpeed();
    check(CubeSpeed != 0.0f);
    GetWorldTimerManager().SetTimer(TeardownTimerHandle, this, &ThisClass::Teardown, (LifeDistance - MovementComponent->GetDistance()) / CubeSpeed);
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
    NiagaraFX->SetVectorParameter(NiagaraVelocityParamName, FVector{-static_cast<double>(MovementComponent->GetSpeed()), 0.0, 0.0});
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
