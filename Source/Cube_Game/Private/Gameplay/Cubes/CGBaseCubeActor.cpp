// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/CGBaseCubeActor.h"
#include "Materials/MaterialInstance.h"

constexpr static float ScalingTimerRate{0.016f};

ACGBaseCubeActor::ACGBaseCubeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    check(StaticMeshComponent);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    StaticMeshComponent->SetGenerateOverlapEvents(true);
    StaticMeshComponent->CastShadow = false;
    SetRootComponent(StaticMeshComponent);
}

void ACGBaseCubeActor::SetColor(const FCubeColorData& InCubeColorData)
{
    CubeColorData = InCubeColorData;
    UMaterialInstanceDynamic* DynMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
    DynMaterial->SetVectorParameterValue(ColorParamName, CubeColorData.Color);
    DynMaterial->SetScalarParameterValue(EmissivePowerParamName, CubeColorData.EmissivePower);
    DynMaterial->SetScalarParameterValue(MaskEnabledParamName, CubeColorData.MaskEnabled);
}

void ACGBaseCubeActor::Teardown()
{
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TargetScale = FVector::ZeroVector;
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ThisClass::OnScaling, ScalingTimerRate, true);
}

UStaticMeshComponent* ACGBaseCubeActor::GetPlayerMesh() const
{
    if (const auto* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
    {
        if (const auto* Pawn = PC->GetPawn())
        {
            return Pawn->FindComponentByClass<UStaticMeshComponent>();
        }
    }

    return nullptr;
}

void ACGBaseCubeActor::BeginPlay()
{
    Super::BeginPlay();

    check(StaticMeshComponent->GetStaticMesh());

    SetActorScale3D(FVector::ZeroVector);
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ThisClass::OnScaling, ScalingTimerRate, true);
}

void ACGBaseCubeActor::OnScaling()
{
    SetActorScale3D(FMath::VInterpConstantTo(GetActorScale3D(), TargetScale, ScalingTimerRate, ScaleInterpSpeed));

    if (GetActorScale3D().IsNearlyZero())
    {
        OnZeroScale();
    }
    else if (GetActorScale3D().Equals(TargetScale))
    {
        GetWorldTimerManager().ClearTimer(ScaleTimerHandle);
        OnScalingDone();
    }
}

void ACGBaseCubeActor::OnScalingDone()
{
}

void ACGBaseCubeActor::OnZeroScale()
{
    Destroy();
}
