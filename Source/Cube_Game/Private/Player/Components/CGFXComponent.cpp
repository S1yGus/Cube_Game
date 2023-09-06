// Cube_Game, All rights reserved.

#include "Player/Components/CGFXComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UCGFXComponent::UCGFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGFXComponent::OutOfPosition()
{
    UGameplayStatics::PlaySound2D(GetWorld(), OutOfPositionSound);
    MakeCameraShake(OutOfPositionCameraShake);
}

void UCGFXComponent::PlayCollectSound(ECubeType CubeType)
{
    if (CollectSoundsMap.Contains(CubeType))
    {
        UGameplayStatics::PlaySound2D(GetWorld(), CollectSoundsMap[CubeType]);
    }
}

void UCGFXComponent::SetCollectColor(ECubeType CubeType)
{
    UStaticMeshComponent* OwnerMesh = GetOwnerMesh();
    if (!OwnerMesh || !CollectColorDataMap.Contains(CubeType))
        return;

    UMaterialInstanceDynamic* DynMaterial = OwnerMesh->CreateAndSetMaterialInstanceDynamic(0);
    check(DynMaterial);
    DynMaterial->SetVectorParameterValue(ColorParamName, CollectColorDataMap[CubeType].Color);
    DynMaterial->SetScalarParameterValue(EmissivePowerParamName, CollectColorDataMap[CubeType].EmissivePower);
    DynMaterial->SetScalarParameterValue(MaskEnabledParamName, CollectColorDataMap[CubeType].MaskEnabled);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(MaterialTimerHandle, this, &ThisClass::OnReturnDefaultColor, TimeOfMaterialChanging);
    }
}

void UCGFXComponent::MakeCameraShake(ECubeType CubeType)
{
    if (!CollectCameraShakeMap.Contains(CubeType))
        return;

    MakeCameraShake(CollectCameraShakeMap[CubeType]);
}

void UCGFXComponent::MakeCameraShake(EBonusType BonusType)
{
    if (!BonusCameraShakeMap.Contains(BonusType))
        return;

    MakeCameraShake(BonusCameraShakeMap[BonusType]);
}

UStaticMeshComponent* UCGFXComponent::GetOwnerMesh() const
{
    return GetOwner() ? GetOwner()->FindComponentByClass<UStaticMeshComponent>() : nullptr;
}

void UCGFXComponent::OnReturnDefaultColor()
{
    SetCollectColor(ECubeType::None);    // None tepe contains default color data.
}

void UCGFXComponent::OnBonusCharged(bool bCharged)
{
    if (!BonusChargedNiagaraComponent)
    {
        BonusChargedNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(BonusChargedNiagaraSystem,        //
                                                                                    GetOwnerMesh(),                   //
                                                                                    NAME_None,                        //
                                                                                    FVector::ZeroVector,              //
                                                                                    FRotator::ZeroRotator,            //
                                                                                    EAttachLocation::SnapToTarget,    //
                                                                                    false);
    }

    if (bCharged)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), BonusChargedSound);
    }

    BonusChargedNiagaraComponent->SetVisibility(bCharged);
}

void UCGFXComponent::MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale)
{
    if (!CameraShakeClass)
        return;

    const auto* OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return;

    const auto* PC = OwnerPawn->GetController<APlayerController>();
    if (!PC || !PC->PlayerCameraManager)
        return;

    PC->PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
}
