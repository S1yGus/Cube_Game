// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CGCoreTypes.h"
#include "CGPlayerCamera.generated.h"

class UCameraComponent;

UCLASS()
class CUBE_GAME_API ACGPlayerCamera : public APawn
{
    GENERATED_BODY()

public:
    ACGPlayerCamera();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComponent;

    virtual void BeginPlay() override;

private:
    void OnAspectRatioChanged(const FAspectRatioData& NewAspectRatioData);
};
