// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/CGPlayerCamera.h"
#include "CGCoreTypes.h"
#include "CGPlayer.generated.h"

class UWidgetComponent;
class UCGBonusComponent;
class UCGFXComponent;

UCLASS()
class CUBE_GAME_API ACGPlayer : public ACGPlayerCamera
{
    GENERATED_BODY()

public:
    ACGPlayer();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* WidgetComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCGBonusComponent* BonusComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCGFXComponent* FXComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control", Meta = (ClampMin = "0"))
    int32 PositionsAmount = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control", Meta = (ClampMin = "0.0"))
    float MovementStep = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control", Meta = (ClampMin = "0.0"))
    float PositionZOffset = 65.0f;

    virtual void BeginPlay() override;

private:
    FTimerHandle MovementTimerHandle;
    int32 CurrentPosition = 0;
    TMap<ECubeType, bool> ReceivingHintsMap;

    inline FVector GetCurrentPositionLocation() const;

    void SetupPlayer();

    void MoveRight();
    void MoveLeft();
    void MoveToCurrentPosition();
    void OnMoving();

    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                 AActor* OtherActor,                          //
                                 UPrimitiveComponent* OtherComp,              //
                                 int32 OtherBodyIndex,                        //
                                 bool bFromSweep,                             //
                                 const FHitResult& SweepResult);              //

    void ReceiveCube(ECubeType CubeType);
    void ShowPopUpHint(ECubeType CubeType);
    void OnHintsStatusChanged(const FHintsStatus& NewHintsStatus);
};
