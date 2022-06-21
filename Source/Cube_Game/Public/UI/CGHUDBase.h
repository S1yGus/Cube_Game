// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CGCoreTypes.h"
#include "CGHUDBase.generated.h"

class UCGAnimatedUserWidget;

UCLASS()
class CUBE_GAME_API ACGHUDBase : public AHUD
{
    GENERATED_BODY()

public:
    virtual void BackToRootMenu(){};

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> OptionsWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> OptionsWarningWidgetClass;

    UPROPERTY()
    TMap<EGameState, UUserWidget*> GameWidgets;

    UPROPERTY()
    UUserWidget* CurrentWidget;

    virtual void BeginPlay() override;
    virtual void SetupWidgets();

private:
    void OnGameStateChanged(EGameState GameState);
};
