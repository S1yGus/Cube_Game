// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/CGCubeActor.h"
#include "Gameplay/Cubes/Components/CGMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CGGameMode.h"

static constexpr float LifeSpan = 2.0f;
constexpr static float LifeDistance = 2500.0f;
constexpr static float UpdatePositionTimerRate = 0.03f;

ACGCubeActor::ACGCubeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
    NiagaraComponent->SetupAttachment(StaticMeshComponent);

    MovementComponent = CreateDefaultSubobject<UCGMovementComponent>("MovementComponent");

    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACGCubeActor::Annihilat()
{
    EndPlayAction();

    auto NiagaraFX = SpawnEndPlayNiagaraEffect(AnnihilationNiagaraSystem);
    NiagaraFX->SetVectorParameter(NiagaraVelocityParamName, FVector{0.0f, static_cast<float>(MovementComponent->GetCubeSpeed()), 0.0f});
}

void ACGCubeActor::SetColor(const FCubeColorData& NewCubeColorData)
{
    Super::SetColor(NewCubeColorData);

    NiagaraComponent->SetColorParameter("Color", NewCubeColorData.Color);
}

void ACGCubeActor::Teardown()
{
    EndPlayAction();

    ReceivingNiagaraComponent = SpawnEndPlayNiagaraEffect(ReceivingNiagaraSystem);

    GetWorldTimerManager().SetTimer(UpdatePositionTimerHandle, this, &ThisClass::OnUpdatePosition, UpdatePositionTimerRate, true);
}

void ACGCubeActor::BeginPlay()
{
    Super::BeginPlay();

    check(NiagaraComponent);
    check(MovementComponent);

    SetLifeSpan(LifeDistance / static_cast<float>(GetCubeSpeed()));
}

int32 ACGCubeActor::GetCubeSpeed() const
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    return GameMode ? GameMode->GetCubeSpeed() : 0;
}

void ACGCubeActor::EndPlayAction()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetVisibility(false, true);
    MovementComponent->StopMoving();
    SetLifeSpan(LifeSpan);
}

UNiagaraComponent* ACGCubeActor::SpawnEndPlayNiagaraEffect(UNiagaraSystem* NiagaraSystem)
{
    const auto NiagaraFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraSystem, GetActorLocation());
    check(NiagaraFX);
    NiagaraFX->SetColorParameter(NiagaraColorParamName, CubeColorData.Color * CubeColorData.EmissivePower);

    return NiagaraFX;
}

void ACGCubeActor::OnUpdatePosition()
{
    const auto PlayerMesh = GetPlayerMesh();
    if (!PlayerMesh || !ReceivingNiagaraComponent)
        return;

    ReceivingNiagaraComponent->SetVectorParameter(NiagaraTargetPositionParamName, PlayerMesh->GetComponentLocation());
}
