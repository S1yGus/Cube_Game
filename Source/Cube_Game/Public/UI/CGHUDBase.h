// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CGCoreTypes.h"
#include "CGHUDBase.generated.h"

UCLASS()
class CUBE_GAME_API ACGHUDBase : public AHUD
{
    GENERATED_BODY()

public:
    TObjectPtr<UUserWidget> GetCurretnWidget() { return CurrentWidget; }
    virtual void BackToRootMenu() {};

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UUserWidget> OptionsWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UUserWidget> OptionsWarningWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UUserWidget> HowToPlayWidgetClass;

    UPROPERTY()
    TMap<EGameState, TObjectPtr<UUserWidget>> GameWidgets;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget;

    virtual void BeginPlay() override;
    virtual void SetupWidgets();

private:
    void OnGameStateChanged(EGameState GameState);
};
