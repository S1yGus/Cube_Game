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
    FOnBonusChargedSignature OnBonusCharged;
    FOnBonusBeginOverlapSignature OnBonusBeginOverlap;

    void CollectBonusCube();
    void UseCurrentBonus();
    EBonusType GetCurrentBonusType() const { return CurrentBonus; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bonus")
    TMap<EBonusType, TSubclassOf<ACGBaseBonusActor>> BonusClassesMap;

private:
    EBonusType CurrentBonus{EBonusType::None};
    bool bBonusCharged{false};

    FORCEINLINE void SetRandomBonus();
    void SetBonus(EBonusType NewBonus);
    void ChargeBonus(bool bCharged);
    void SpawnBonus(EBonusType BonusType);
};
