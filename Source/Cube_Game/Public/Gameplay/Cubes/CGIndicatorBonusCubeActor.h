// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/CGBaseCubeActor.h"
#include "CGIndicatorBonusCubeActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGIndicatorBonusCubeActor : public ACGBaseCubeActor
{
    GENERATED_BODY()

public:
    void SetBonusType(EBonusType NewBonusType) { BonusType = NewBonusType; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
    UMaterialInstance* DefaultMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
    TMap<EBonusType, UMaterialInstance*> MaterialsMap;

    EBonusType BonusType = EBonusType::None;

    virtual void BeginPlay() override;

private:
    void SetMaterial();
};
