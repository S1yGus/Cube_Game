// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/CGCubeActor.h"
#include "Gameplay/Cubes/Components/CGMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CGGameMode.h"

constexpr static float CubeLifeSpan{2.0f};
constexpr static float UpdatePositionTimerRate{0.03f};

ACGCubeActor::ACGCubeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    TrailNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Trail");
    TrailNiagaraComponent->SetupAttachment(StaticMeshComponent);

    MovementComponent = CreateDefaultSubobject<UCGMovementComponent>("MovementComponent");

    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACGCubeActor::Annihilat()
{
    EndPlayAction();
    UNiagaraComponent* NiagaraFX = SpawnNiagaraEffect(AnnihilatNiagaraSystem);
    NiagaraFX->SetVectorParameter(NiagaraVelocityParamName, FVector{-static_cast<double>(MovementComponent->GetCubeSpeed()), 0.0, 0.0});
}

void ACGCubeActor::Collect()
{
    EndPlayAction();
    CollectNiagaraComponent = SpawnNiagaraEffect(CollectNiagaraSystem);
    GetWorldTimerManager().SetTimer(UpdateNiagaraTargetPositionTimerHandle, this, &ThisClass::OnUpdateNiagaraTargetPosition, UpdatePositionTimerRate, true);
}

void ACGCubeActor::SetColor(const FCubeColorData& NewCubeColorData)
{
    Super::SetColor(NewCubeColorData);

    TrailNiagaraComponent->SetColorParameter(NiagaraColorParamName, NewCubeColorData.Color);
}

void ACGCubeActor::BeginPlay()
{
    Super::BeginPlay();

    check(TrailNiagaraComponent);
    check(MovementComponent);

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
    SetLifeSpan(CubeLifeSpan);
}

UNiagaraComponent* ACGCubeActor::SpawnNiagaraEffect(TObjectPtr<UNiagaraSystem> NiagaraSystem)
{
    UNiagaraComponent* NiagaraFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraSystem, GetActorLocation());
    check(NiagaraFX);
    NiagaraFX->SetColorParameter(NiagaraColorParamName, CubeColorData.Color * CubeColorData.EmissivePower);

    return NiagaraFX;
}

void ACGCubeActor::OnUpdateNiagaraTargetPosition()
{
    const auto* PlayerMesh = GetPlayerMesh();
    if (!PlayerMesh || !CollectNiagaraComponent)
        return;

    CollectNiagaraComponent->SetVectorParameter(NiagaraTargetPositionParamName, PlayerMesh->GetComponentLocation());
}
