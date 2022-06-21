// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGHUDBase.h"
#include "CGHUDMenu.generated.h"

class UCGAnimatedUserWidget;

UCLASS()
class CUBE_GAME_API ACGHUDMenu : public ACGHUDBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> DifficultySelectionWidgetClass;

    virtual void BackToRootMenu() override;

private:
    virtual void SetupWidgets() override;
};
