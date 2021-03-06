// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGCoreTypes.h"
#include "CGBonusComponent.generated.h"

class ACGBaseBonusActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBE_GAME_API UCGBonusComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCGBonusComponent();

    FOnBonusChangedSignature OnBonusChanged;
    FOnPowerupedSignature OnPoweruped;

    void SetRandomBonus();
    void UseCurrentBonus();
    void UseRandomPowerup();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bonus")
    TMap<EBonusType, TSubclassOf<ACGBaseBonusActor>> BonusClassesMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Powerup|Uber", Meta = (ClampMin = "0.0"))
    float UberDuration = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Powerup|Uber", Meta = (ClampMin = "0.0"))
    float UberFireFrequency = 0.5f;

private:
    FTimerHandle UberTimerHandle;
    float UberTime = 0;

    EBonusType Bonus = EBonusType::None;

    void SetBonus(EBonusType NewBonus);
    ACGBaseBonusActor* SpawnBonus(EBonusType BonusType);

    void UseUberPowerup();
    void OnUberPowerupFired();
};
