// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGCoreTypes.h"
#include "CGMovementComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBE_GAME_API UCGMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCGMovementComponent();

    void SetSpeedData(const FSpeedData& SpeedData);
    void SetGameSpeed(int32 GameSpeed);
    float GetSpeed() const;
    float GetDistance() const { return Distance; }
    void StopMoving();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    bool bOverrideSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (Units = "CentimetersPerSecond", EditCondition = "bOverrideSpeed"))
    float Speed{0.0f};

    virtual void BeginPlay() override;

private:
    FTimerHandle MovementTimerHandle;
    FSpeedData CurrentSpeedData;
    int32 CurrentGameSpeed{0};
    float Distance{0.0f};

    void OnMoving();
};
