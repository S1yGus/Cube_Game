// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGCoreTypes.h"
#include "CGFXComponent.generated.h"

class USoundCue;
class UMaterialInstance;
class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBE_GAME_API UCGFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCGFXComponent();

    void PlayReceivingSound(ECubeType CubeType);
    void SetReceivingMaterial(ECubeType CubeType);
    void MakeCameraShake(ECubeType CubeType);
    void MakeCameraShake(EBonusType BonusType);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TMap<ECubeType, USoundCue*> ReceivingSoundsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    UMaterialInstance* DefaultMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    TMap<ECubeType, UMaterialInstance*> ReceivingMaterialsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TMap<ECubeType, TSubclassOf<UCameraShakeBase>> CubesCameraShakeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TMap<EBonusType, TSubclassOf<UCameraShakeBase>> BonusesCameraShakeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material", Meta = (ClampMin = "0.0"))
    float TimeOfMaterialChanging = 0.25f;

private:
    FTimerHandle MaterialTimerHandle;

    inline UStaticMeshComponent* GetOwnerMesh() const;

    void OnReturnDefaultMaterial();

    void MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale = 1.0f);
};
