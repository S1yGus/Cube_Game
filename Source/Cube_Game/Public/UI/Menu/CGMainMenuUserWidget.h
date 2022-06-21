// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "Interfaces/CGWidgetInterface.h"
#include "CGCoreTypes.h"
#include "CGMainMenuUserWidget.generated.h"

class UCGButtonUserWidget;

UCLASS()
class CUBE_GAME_API UCGMainMenuUserWidget : public UCGAnimatedUserWidget, public ICGWidgetInterface
{
    GENERATED_BODY()

public:
    virtual void ResetWidget() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* GameButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* OptionsButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* QuitButton;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<UCGButtonUserWidget*> WidgetButtons;

    EGameState GameStateToSet = EGameState::WaitingToStart;

    void OnClickedGameButton();
    void OnClickedOptionsButton();
    void OnClickedQuitButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
