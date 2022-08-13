// Cube_Game, All rights reserved.

#include "Player/Components/CGBonusComponent.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Player/Components/CGFXComponent.h"

constexpr static int32 MaxChargeLevel = 2;

UCGBonusComponent::UCGBonusComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGBonusComponent::SetRandomBonus()
{
    if (CurrentBonus != EBonusType::None && !bBonusCharged)
    {
        SetBonusCharged(true);
    }
    else
    {
        const auto RandBonusNum = FMath::RandHelper(static_cast<int32>(EBonusType::Max) - 1) + 1;    // - 1 and + 1 to avoid EBonusType::None
        SetBonus(static_cast<EBonusType>(RandBonusNum));
        SetBonusCharged(false);
    }
}

void UCGBonusComponent::UseCurrentBonus()
{
    if (CurrentBonus == EBonusType::None)
        return;

    SpawnBonus(CurrentBonus);
    SetBonus(EBonusType::None);
    SetBonusCharged(false);
}

void UCGBonusComponent::SetBonus(EBonusType NewBonus)
{
    if (CurrentBonus == NewBonus)
        return;

    CurrentBonus = NewBonus;
    OnBonusChanged.Broadcast(NewBonus);
}

void UCGBonusComponent::SetBonusCharged(bool IsCharged)
{
    if (bBonusCharged == IsCharged)
        return;

    bBonusCharged = IsCharged;
    OnBonusCharged.Broadcast(bBonusCharged);
}

void UCGBonusComponent::SpawnBonus(EBonusType BonusType)
{
    if (!BonusClassesMap.Contains(BonusType) || !GetOwner())
        return;

    const auto PlayrMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    if (!PlayrMesh)
        return;

    const auto SpawnTransform = PlayrMesh->GetComponentTransform();
    const auto BonusActor = GetWorld()->SpawnActorDeferred<ACGBaseBonusActor>(BonusClassesMap[BonusType], SpawnTransform);
    check(BonusActor);
    BonusActor->SetBonusCharged(bBonusCharged);
    BonusActor->SetOwner(GetOwner());
    BonusActor->FinishSpawning(SpawnTransform);

    if (const auto FXComponent = GetOwner()->FindComponentByClass<UCGFXComponent>())
    {
        FXComponent->MakeCameraShake(BonusType);
    }
}
