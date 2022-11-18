// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGMovementComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBE_GAME_API UCGMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCGMovementComponent();

    void StopMoving();
    int32 GetCubeSpeed() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    bool bGetSpeedFromGameMode;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (Units = "CentimetersPerSecond", EditCondition = "!bGetSpeedFromGameMode"))
    int32 Speed = 0;

    virtual void BeginPlay() override;

private:
    FTimerHandle MovementTimerHandle;

    void OnMoving();
};
