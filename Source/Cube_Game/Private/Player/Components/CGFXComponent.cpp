// Cube_Game, All rights reserved.

#include "Player/Components/CGFXComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UCGFXComponent::UCGFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGFXComponent::OutOfPosition()
{
    UGameplayStatics::PlaySound2D(GetWorld(), OutOfPositionSounds);

    MakeCameraShake(OutOfPositionCameraShake);
}

void UCGFXComponent::PlayReceivingSound(ECubeType CubeType)
{
    if (!ReceivingSoundsMap.Contains(CubeType))
        return;

    UGameplayStatics::PlaySound2D(GetWorld(), ReceivingSoundsMap[CubeType]);
}

void UCGFXComponent::SetReceivingMaterial(ECubeType CubeType)
{
    if (!ReceivingMaterialsMap.Contains(CubeType) || !GetOwnerMesh())
        return;

    GetOwnerMesh()->SetMaterial(0, ReceivingMaterialsMap[CubeType]);

    GetWorld()->GetTimerManager().SetTimer(MaterialTimerHandle, this, &UCGFXComponent::OnReturnDefaultMaterial, TimeOfMaterialChanging);
}

void UCGFXComponent::MakeCameraShake(ECubeType CubeType)
{
    if (!CubesCameraShakeMap.Contains(CubeType))
        return;

    MakeCameraShake(CubesCameraShakeMap[CubeType]);
}

void UCGFXComponent::MakeCameraShake(EBonusType BonusType)
{
    if (!BonusesCameraShakeMap.Contains(BonusType))
        return;

    MakeCameraShake(BonusesCameraShakeMap[BonusType]);
}

UStaticMeshComponent* UCGFXComponent::GetOwnerMesh() const
{
    return GetOwner() ? GetOwner()->FindComponentByClass<UStaticMeshComponent>() : nullptr;
}

void UCGFXComponent::OnReturnDefaultMaterial()
{
    if (!DefaultMaterial || !GetOwnerMesh())
        return;

    GetOwnerMesh()->SetMaterial(0, DefaultMaterial);
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
