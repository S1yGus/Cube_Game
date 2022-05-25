// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Cubes/CGBaseCubeActor.h"
#include "CGIndicatorCubeActor.generated.h"

UCLASS()
class CUBE_GAME_API ACGIndicatorCubeActor : public ACGBaseCubeActor
{
    GENERATED_BODY()

public:
    void SetCubeType(ECubeType NewCubeType) { CubeType = NewCubeType; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
    UMaterialInstance* DefaultMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
    TMap<ECubeType, UMaterialInstance*> MaterialsMap;

    ECubeType CubeType = ECubeType::None;

    virtual void BeginPlay() override;

private:
    void SetMaterial();
};
