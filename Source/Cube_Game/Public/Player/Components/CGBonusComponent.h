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

    void SetRandomBonus();
    void UseCurrentBonus();
    void UseRandomPowerup();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bonus")
    TMap<EBonusType, TSubclassOf<ACGBaseBonusActor>> BonusClassesMap;

private:
    EBonusType Bonus = EBonusType::None;

    void SetBonus(EBonusType NewBonus);
    ACGBaseBonusActor* SpawnBonus(EBonusType BonusType);
};
