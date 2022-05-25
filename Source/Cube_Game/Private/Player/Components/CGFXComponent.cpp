// Cube_Game, All rights reserved.

#include "Player/Components/CGFXComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UCGFXComponent::UCGFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
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
