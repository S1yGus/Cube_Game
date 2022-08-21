// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGCoreTypes.h"
#include "CGFXComponent.generated.h"

class USoundCue;
class UMaterialInstance;
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
    void PlaySoundOfReceiving(ECubeType CubeType);
    void SetColorOfReceiving(ECubeType CubeType);
    void MakeCameraShake(ECubeType CubeType);
    void MakeCameraShake(EBonusType BonusType);

    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* BonusChargedSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* OutOfPositionSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TMap<ECubeType, USoundCue*> SoundsOfReceivingMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material", Meta = (ToolTip = "Use none type for default color data."))
    TMap<ECubeType, FCubeColorData> ColorDataOfReceivingMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName ColorParamName = "EmissiveColor";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName EmissivePowerParamName = "EmissivePower";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName MaskEnabledParamName = "MaskEnabled";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material", Meta = (ClampMin = "0.0"))
    float TimeOfMaterialChanging = 0.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TMap<ECubeType, TSubclassOf<UCameraShakeBase>> CameraShakeOfReceivingMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TMap<EBonusType, TSubclassOf<UCameraShakeBase>> CameraShakeOfBonusesMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TSubclassOf<UCameraShakeBase> OutOfPositionCameraShake;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
    UNiagaraSystem* BonusChargedNiagaraSystem;

private:
    FTimerHandle MaterialTimerHandle;

    UPROPERTY()
    UNiagaraComponent* BonusChargedNiagaraComponent;

    inline UStaticMeshComponent* GetOwnerMesh() const;

    void Setup();

    void OnReturnDefaultColor();
    void OnBonusCharged(bool IsCharged);

    void MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale = 1.0f);
};
