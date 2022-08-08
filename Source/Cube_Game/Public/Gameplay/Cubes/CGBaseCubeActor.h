// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGCoreTypes.h"
#include "CGBaseCubeActor.generated.h"

class UMaterialInstance;

UCLASS()
class CUBE_GAME_API ACGBaseCubeActor : public AActor
{
    GENERATED_BODY()

public:
    ACGBaseCubeActor();

    virtual void SetColor(const FCubeColorData& NewCubeColorData);
    virtual void Teardown();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scale", Meta = (ClampMin = "0.0"))
    float ScaleInterpSpeed = 9.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scale")
    FVector TargetScale{1.0};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName ColorParamName = "EmissiveColor";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName EmissivePowerParamName = "EmissivePower";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName MaskEnabledParamName = "MaskEnabled";

    FCubeColorData CubeColorData;

    UStaticMeshComponent* GetPlayerMesh() const;

    virtual void BeginPlay() override;
    virtual void OnScalingZero();
    virtual void OnScalingDone();
    virtual void OnScalin();

private:
    FTimerHandle ScaleTimerHandle;

    void OnChangingScale();
};
