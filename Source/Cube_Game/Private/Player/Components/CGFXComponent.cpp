// Cube_Game, All rights reserved.

#include "Player/Components/CGFXComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Player/Components/CGBonusComponent.h"
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

void UCGFXComponent::PlaySoundOfReceiving(ECubeType CubeType)
{
    if (!SoundsOfReceivingMap.Contains(CubeType))
        return;

    UGameplayStatics::PlaySound2D(GetWorld(), SoundsOfReceivingMap[CubeType]);
}

void UCGFXComponent::SetColorOfReceiving(ECubeType CubeType)
{
    const auto OwnerMesh = GetOwnerMesh();
    if (!ColorDataOfReceivingMap.Contains(CubeType) || !OwnerMesh)
        return;

    const auto DynMaterial = OwnerMesh->CreateAndSetMaterialInstanceDynamic(0);
    check(DynMaterial);
    DynMaterial->SetVectorParameterValue(ColorParamName, ColorDataOfReceivingMap[CubeType].Color);
    DynMaterial->SetScalarParameterValue(EmissivePowerParamName, ColorDataOfReceivingMap[CubeType].EmissivePower);
    DynMaterial->SetScalarParameterValue(MaskEnabledParamName, ColorDataOfReceivingMap[CubeType].MaskEnabled);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(MaterialTimerHandle, this, &ThisClass::OnReturnDefaultColor, TimeOfMaterialChanging);
    }
}

void UCGFXComponent::MakeCameraShake(ECubeType CubeType)
{
    if (!CameraShakeOfReceivingMap.Contains(CubeType))
        return;

    MakeCameraShake(CameraShakeOfReceivingMap[CubeType]);
}

void UCGFXComponent::MakeCameraShake(EBonusType BonusType)
{
    if (!CameraShakeOfBonusesMap.Contains(BonusType))
        return;

    MakeCameraShake(CameraShakeOfBonusesMap[BonusType]);
}

void UCGFXComponent::BeginPlay()
{
    Super::BeginPlay();

    Setup();
}

UStaticMeshComponent* UCGFXComponent::GetOwnerMesh() const
{
    return GetOwner() ? GetOwner()->FindComponentByClass<UStaticMeshComponent>() : nullptr;
}

void UCGFXComponent::Setup()
{
    const auto Owner = GetOwner<APawn>();
    if (!Owner)
        return;

    if (const auto BonusComponwnt = Owner->FindComponentByClass<UCGBonusComponent>())
    {
        BonusComponwnt->OnBonusCharged.AddUObject(this, &ThisClass::OnBonusCharged);
    }
}

void UCGFXComponent::OnReturnDefaultColor()
{
    SetColorOfReceiving(ECubeType::None);    // None tepe contains default color data.
}

void UCGFXComponent::OnBonusCharged(bool IsCharged)
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

    if (IsCharged)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), BonusChargedSound);
    }

    BonusChargedNiagaraComponent->SetVisibility(IsCharged);
}

void UCGFXComponent::MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale)
{
    if (!CameraShakeClass)
        return;

    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return;

    const auto PlayerController = OwnerPawn->GetController<APlayerController>();
    if (!PlayerController || !PlayerController->PlayerCameraManager)
        return;

    PlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
}
