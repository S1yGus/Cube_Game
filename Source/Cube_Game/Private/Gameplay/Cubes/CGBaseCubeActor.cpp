// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/CGBaseCubeActor.h"

constexpr static float ScalingTimerRate = 0.016f;

ACGBaseCubeActor::ACGBaseCubeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->CastShadow = false;
    SetRootComponent(StaticMeshComponent);
}

void ACGBaseCubeActor::Teardown()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TargetScale = FVector::ZeroVector;
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ACGBaseCubeActor::OnChangingScale, ScalingTimerRate, true);
}

void ACGBaseCubeActor::BeginPlay()
{
    Super::BeginPlay();

    SetActorScale3D(FVector::ZeroVector);
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ACGBaseCubeActor::OnChangingScale, ScalingTimerRate, true);
}

void ACGBaseCubeActor::OnScalingDone()
{
}

void ACGBaseCubeActor::OnScalin()
{
}

void ACGBaseCubeActor::OnChangingScale()
{
    SetActorScale3D(FMath::VInterpConstantTo(GetActorScale3D(), TargetScale, ScalingTimerRate, ScaleInterpSpeed));

    if (FMath::IsNearlyZero(GetActorScale3D().X)        //
        && FMath::IsNearlyZero(GetActorScale3D().Y)     //
        && FMath::IsNearlyZero(GetActorScale3D().Z))    //
    {
        Destroy();
    }
    else if (FMath::IsNearlyEqual(GetActorScale3D().X, TargetScale.X)        //
             && FMath::IsNearlyEqual(GetActorScale3D().Y, TargetScale.Y)     //
             && FMath::IsNearlyEqual(GetActorScale3D().Z, TargetScale.Z))    //
    {
        GetWorldTimerManager().ClearTimer(ScaleTimerHandle);
        OnScalingDone();
    }

    OnScalin();
}
