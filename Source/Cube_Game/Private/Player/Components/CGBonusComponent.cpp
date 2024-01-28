// Cube_Game, All rights reserved.

#include "Player/Components/CGBonusComponent.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"

UCGBonusComponent::UCGBonusComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGBonusComponent::CollectBonusCube()
{
    if (CurrentBonus != EBonusType::None && !bBonusCharged)
    {
        ChargeBonus(true);
    }
    else
    {
        SetRandomBonus();
    }
}

void UCGBonusComponent::UseCurrentBonus()
{
    if (CurrentBonus == EBonusType::None)
        return;

    SpawnBonus(CurrentBonus);
    SetBonus(EBonusType::None);
    ChargeBonus(false);
}

void UCGBonusComponent::SetRandomBonus()
{
    int32 RandBonusNum{0};
    if (CurrentBonus == EBonusType::None)
    {
        RandBonusNum = FMath::RandHelper(static_cast<int32>(EBonusType::Max) - 1);    // - 1 for none type
    }
    else
    {
        const int32 ToAdd = FMath::RandRange(1, static_cast<int32>(EBonusType::Max) - 2);    // - 2 for none type and current type
        RandBonusNum = (static_cast<int32>(CurrentBonus) + ToAdd) % (static_cast<int32>(EBonusType::Max) - 1);
    }

    SetBonus(static_cast<EBonusType>(RandBonusNum));
    ChargeBonus(false);
}

void UCGBonusComponent::SetBonus(EBonusType NewBonus)
{
    if (CurrentBonus == NewBonus)
        return;

    CurrentBonus = NewBonus;
    OnBonusChanged.Broadcast(NewBonus);
}

void UCGBonusComponent::ChargeBonus(bool bCharged)
{
    if (bBonusCharged == bCharged)
        return;

    bBonusCharged = bCharged;
    OnBonusCharged.Broadcast(bBonusCharged);
}

void UCGBonusComponent::SpawnBonus(EBonusType BonusType)
{
    if (!GetWorld() || !GetOwner() || !BonusClassesMap.Contains(BonusType))
        return;

    const auto* PlayrMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    if (!PlayrMesh)
        return;

    const auto SpawnTransform = PlayrMesh->GetComponentTransform();
    auto* BonusActor = GetWorld()->SpawnActorDeferred<ACGBaseBonusActor>(BonusClassesMap[BonusType], SpawnTransform);
    check(BonusActor);
    BonusActor->SetBonusCharged(bBonusCharged);
    BonusActor->SetOwner(GetOwner());
    BonusActor->FinishSpawning(SpawnTransform);
}
