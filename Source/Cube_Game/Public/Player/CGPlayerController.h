// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CGCoreTypes.h"
#include "CGPlayerController.generated.h"

class ACGGameModeBase;
class UInputAction;
class UInputMappingContext;

UCLASS()
class CUBE_GAME_API ACGPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    FOnPressedEnterSignature OnPressedEnter;
    FOnPressedEscSignature OnPressedEscape;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> EnterAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> EscapeAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    virtual void SetupInputComponent() override;
    virtual void BeginPlay() override;

private:
    FORCEINLINE TObjectPtr<ACGGameModeBase> GetGameModeBase() const;

    void OnGameStateChanged(EGameState NewGameState);
    void OnPressEnter();
    void OnPressEscape();
};
