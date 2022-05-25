// Cube_Game, All rights reserved.

#include "Player/Components/CGBonusComponent.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Player/CGPlayer.h"
#include "Gameplay/Cubes/Bonuses/CGShieldBonusActor.h"
#include "CGGameMode.h"

UCGBonusComponent::UCGBonusComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGBonusComponent::SetRandomBonus()
{
    const auto RandBonusNum = FMath::RandHelper(static_cast<int32>(EBonusType::Max));
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

void UCGBonusComponent::UseRandomPowerup()
{
    const auto RandPowerupType = static_cast<EPowerupType>(FMath::RandHelper(static_cast<int32>(EPowerupType::Max)));
    OnPoweruped.Broadcast(RandPowerupType);

    switch (RandPowerupType)
    {
        case EPowerupType::Uber:
            UseUberPowerup();
            break;
        case EPowerupType::Speed:
            UseSpeedPowerup();
            break;
        default:
            break;
    }
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
    if (!BonusesMap.Contains(BonusType) || !GetOwner())
        return nullptr;

    const auto PlayrMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    if (!PlayrMesh)
        return nullptr;

    return GetWorld()->SpawnActor<ACGBaseBonusActor>(BonusesMap[BonusType], PlayrMesh->GetComponentTransform());
}

void UCGBonusComponent::UseUberPowerup()
{
    if (!BonusesMap.Contains(EBonusType::Shield) || !GetOwner())
        return;

    const auto PlayrMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    if (!PlayrMesh)
        return;

    const auto Shield = GetWorld()->SpawnActorDeferred<ACGShieldBonusActor>(BonusesMap[EBonusType::Shield], PlayrMesh->GetComponentTransform());
    if (!Shield)
        return;

    Shield->SetDuration(UberDuration);
    Shield->FinishSpawning(PlayrMesh->GetComponentTransform());

    GetWorld()->GetTimerManager().SetTimer(UberTimerHandle, this, &UCGBonusComponent::OnUberPowerupFired, UberFireFrequency, true);
}

void UCGBonusComponent::OnUberPowerupFired()
{
    if (UberTime >= UberDuration)
    {
        GetWorld()->GetTimerManager().ClearTimer(UberTimerHandle);
        UberTime = 0;
        return;
    }

    UberTime += UberFireFrequency;
    SpawnBonus(EBonusType::Missile);
}

void UCGBonusComponent::UseSpeedPowerup()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode)
        return;

    PreviousSpeed = GameMode->GetSpeed();
    GameMode->ChangeSpeed(PreviousSpeed * SpeedMultiplier);

    GetWorld()->GetTimerManager().SetTimer(SpeedTimerHandle, this, &UCGBonusComponent::OnSpeedPowerupFinished, SpeedDuration);
}

void UCGBonusComponent::OnSpeedPowerupFinished()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode || !GetOwner())
        return;

    const auto DifficultyVlues = GameMode->GetDifficultyVlues();
    if (!DifficultyVlues)
        return;

    const auto CurrentSpeed = GameMode->GetScore() / DifficultyVlues->ScoreToSpeedUp;
    GameMode->ChangeSpeed(CurrentSpeed > PreviousSpeed ? CurrentSpeed : PreviousSpeed);
}
