// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/CGPlayerCamera.h"
#include "CGCoreTypes.h"
#include "CGPlayer.generated.h"

class UWidgetComponent;
class UCGBonusComponent;
class UCGFXComponent;
class UInputAction;

UCLASS()
class CUBE_GAME_API ACGPlayer : public ACGPlayerCamera
{
    GENERATED_BODY()

public:
    ACGPlayer();

    void UpdateLocation(const FTransform& InOrigin, const FVector& InFieldSize);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UWidgetComponent> WidgetComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCGBonusComponent> BonusComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCGFXComponent> FXComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control", Meta = (ClampMin = "0"))
    int32 PositionsAmount{4};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform", Meta = (ClampMin = "0.0", Units = "cm"))
    float MovementStep{200.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform", Meta = (Units = "cm"))
    float PositionXOffset{100.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform", Meta = (Units = "cm"))
    float PositionYOffset{100.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform", Meta = (Units = "cm"))
    float PositionZOffset{65.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform", Meta = (Units = "cm"))
    float FieldMargin{300.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> UseCurrentBonusAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> MoveRightAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> MoveLeftAction;

    virtual void BeginPlay() override;

private:
    FTimerHandle MovementTimerHandle;
    FDelegateHandle OnViewportResizedHandle;
    FTransform Origin{FTransform::Identity};
    FVector FieldSize{0.0};
    int32 CurrentPosition{0};

    FORCEINLINE FVector GetTargetPositionLocation() const;

    void SetupPlayer();

    void OnUseCurrentBonus();
    void OnMoveRight();
    void OnMoveLeft();
    FORCEINLINE void StartMovingToCurrentPosition();
    void OnMoving();
    UFUNCTION()
    void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                              AActor* OtherActor,                          //
                              UPrimitiveComponent* OtherComp,              //
                              int32 OtherBodyIndex,                        //
                              bool bFromSweep,                             //
                              const FHitResult& SweepResult);              //
    void OnBonusBeginOverlap(ACGCubeActor* Cube, bool bCharged);
    void OnViewportResized(FViewport* Viewport, uint32 Value);

    FORCEINLINE void ProduceCollectEffects(ECubeType CubeType);
    FORCEINLINE void UpdateGameMode(ECubeType CubeType);
    void CollectCube(ACGCubeActor* Cube);
};
