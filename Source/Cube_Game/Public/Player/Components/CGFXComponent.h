// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGCoreTypes.h"
#include "CGFXComponent.generated.h"

class USoundCue;
class UCameraShakeBase;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBE_GAME_API UCGFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCGFXComponent();

    void OutOfPosition();
    void PlayCollectSound(ECubeType CubeType);
    void SetCollectColor(ECubeType CubeType);
    void MakeCameraShake(ECubeType CubeType);
    void MakeCameraShake(EBonusType BonusType);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> BonusChargedSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> OutOfPositionSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TMap<ECubeType, TObjectPtr<USoundCue>> CollectSoundsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material", Meta = (ToolTip = "Use none type for default color data."))
    TMap<ECubeType, FCubeColorData> CollectColorDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName ColorParamName{"EmissiveColor"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName EmissivePowerParamName{"EmissivePower"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName MaskEnabledParamName{"MaskEnabled"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material", Meta = (ClampMin = "0.0"))
    float TimeOfMaterialChanging{0.3f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TMap<ECubeType, TSubclassOf<UCameraShakeBase>> CollectCameraShakeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TMap<EBonusType, TSubclassOf<UCameraShakeBase>> BonusCameraShakeMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TSubclassOf<UCameraShakeBase> OutOfPositionCameraShake;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
    TObjectPtr<UNiagaraSystem> BonusChargedNiagaraSystem;

private:
    FTimerHandle MaterialTimerHandle;

    UPROPERTY()
    TObjectPtr<UNiagaraComponent> BonusChargedNiagaraComponent;

    FORCEINLINE UStaticMeshComponent* GetOwnerMesh() const;

    void OnReturnDefaultColor();
    void OnBonusCharged(bool bCharged);

    void MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale = 1.0f);

    friend class ACGPlayer;
};
