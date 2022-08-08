// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/CGBaseCubeActor.h"
#include "Materials/MaterialInstance.h"

constexpr static float ScalingTimerRate = 0.016f;

ACGBaseCubeActor::ACGBaseCubeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->CastShadow = false;
    SetRootComponent(StaticMeshComponent);
}

void ACGBaseCubeActor::SetColor(const FCubeColorData& NewCubeColorData)
{
    CubeColorData = NewCubeColorData;
    const auto DynMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
    DynMaterial->SetVectorParameterValue(ColorParamName, CubeColorData.Color);
    DynMaterial->SetScalarParameterValue(EmissivePowerParamName, CubeColorData.EmissivePower);
    DynMaterial->SetScalarParameterValue(MaskEnabledParamName, CubeColorData.MaskEnabled);
}

void ACGBaseCubeActor::Teardown()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TargetScale = FVector::ZeroVector;
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ThisClass::OnChangingScale, ScalingTimerRate, true);
}

UStaticMeshComponent* ACGBaseCubeActor::GetPlayerMesh() const
{
    const auto PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
        return nullptr;

    const auto PlayerPawn = PlayerController->GetPawn();
    if (!PlayerPawn)
        return nullptr;

    return PlayerPawn->FindComponentByClass<UStaticMeshComponent>();
}

void ACGBaseCubeActor::BeginPlay()
{
    Super::BeginPlay();

    check(StaticMeshComponent);

    SetActorScale3D(FVector::ZeroVector);
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ThisClass::OnChangingScale, ScalingTimerRate, true);
}

void ACGBaseCubeActor::OnScalingZero()
{
    Destroy();
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
        OnScalingZero();
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
