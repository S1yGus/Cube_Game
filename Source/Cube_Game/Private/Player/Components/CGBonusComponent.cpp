// Cube_Game, All rights reserved.

#include "Player/Components/CGBonusComponent.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Player/Components/CGFXComponent.h"

UCGBonusComponent::UCGBonusComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGBonusComponent::SetRandomBonus()
{
    const auto RandBonusNum = FMath::RandHelper(static_cast<int32>(EBonusType::Max) - 1) + 1;
    SetBonus(static_cast<EBonusType>(RandBonusNum));
}

void UCGBonusComponent::UseCurrentBonus()
{
    if (Bonus == EBonusType::None)
        return;

    const auto SpawnedBonus = SpawnBonus(Bonus);
    if (!SpawnedBonus)
        return;

    SetBonus(EBonusType::None);
}

void UCGBonusComponent::SetBonus(EBonusType NewBonus)
{
    if (Bonus == NewBonus)
        return;

    Bonus = NewBonus;
    OnBonusChanged.Broadcast(NewBonus);
}

ACGBaseBonusActor* UCGBonusComponent::SpawnBonus(EBonusType BonusType)
{
    if (!BonusClassesMap.Contains(BonusType) || !GetOwner())
        return nullptr;

    const auto PlayrMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    if (!PlayrMesh)
        return nullptr;

    if (const auto FXComponent = GetOwner()->FindComponentByClass<UCGFXComponent>())
    {
        FXComponent->MakeCameraShake(BonusType);
    }

    return GetWorld()->SpawnActor<ACGBaseBonusActor>(BonusClassesMap[BonusType], PlayrMesh->GetComponentTransform());
}
