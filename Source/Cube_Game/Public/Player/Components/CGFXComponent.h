// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGCoreTypes.h"
#include "CGFXComponent.generated.h"

class USoundCue;
class UMaterialInstance;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBE_GAME_API UCGFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCGFXComponent();

    void PlayReceivingSound(ECubeType CubeType);
    void SetReceivingMaterial(ECubeType CubeType);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TMap<ECubeType, USoundCue*> ReceivingSoundsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    UMaterialInstance* DefaultMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    TMap<ECubeType, UMaterialInstance*> ReceivingMaterialsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    float TimeOfMaterialChanging = 2.0f;

private:
    FTimerHandle MaterialTimerHandle;

    UStaticMeshComponent* GetOwnerMesh() const;
    void OnReturnDefaultMaterial();
};
