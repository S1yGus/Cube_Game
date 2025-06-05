// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGHUDBase.h"
#include "CGHUDGame.generated.h"

UCLASS()
class CUBE_GAME_API ACGHUDGame : public ACGHUDBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UUserWidget> PopUpHintWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    virtual void BeginPlay() override;
    virtual void BackToRootMenu() override;

private:
    virtual void SetupWidgets() override;
};
