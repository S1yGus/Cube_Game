// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/CGPlayerCamera.h"
#include "CGCoreTypes.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control", Meta = (ClampMin = "0.0", Units = "cm"))
    float MovementStep{200.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control", Meta = (ClampMin = "0.0", Units = "cm"))
    float PositionZOffset{65.0f};

    virtual void BeginPlay() override;

private:
    FTimerHandle MovementTimerHandle;
    int32 CurrentPosition{0};

    FORCEINLINE FVector GetCurrentPositionLocation() const;

    void SetupPlayer();

    void MoveRight();
    void MoveLeft();
    FORCEINLINE void MoveToCurrentPosition();
    void OnMoving();
    void UseCurrentBonus();

    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                 AActor* OtherActor,                          //
                                 UPrimitiveComponent* OtherComp,              //
                                 int32 OtherBodyIndex,                        //
                                 bool bFromSweep,                             //
                                 const FHitResult& SweepResult);              //

    void CollectCube(ECubeType CubeType);

    friend void ACGBaseBonusActor::NotifyActorBeginOverlap(AActor* OtherActor);
};
