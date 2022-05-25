// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/CGPlayerCamera.h"
#include "CGCoreTypes.h"
#include "CGPlayer.generated.h"

class USoundCue;
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
    int32 PositionsAmount = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* OutOfPosition;

    virtual void BeginPlay() override;

private:
    FTimerHandle MovementTimerHandle;

    int32 CurrentPosition = 0;

    void SetupPlayer();

    inline FVector GetCurrentPositionLocation() const;

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
};
