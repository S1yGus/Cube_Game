// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "Interfaces/CGWidgetInterface.h"
#include "CGCoreTypes.h"
#include "CGPauseUserWidget.generated.h"

class UCGButtonUserWidget;

UCLASS()
class CUBE_GAME_API UCGPauseUserWidget : public UCGAnimatedUserWidget, public ICGWidgetInterface
{
    GENERATED_BODY()

public:
    virtual void ResetWidget() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* ResumeButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* OptionsButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* MenuButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* QuitButton;

    virtual void NativeOnInitialized() override;
    virtual void SetGameState(EGameState NewGameState) override;

private:
    UPROPERTY()
    TArray<UCGButtonUserWidget*> WidgetButtons;

    EGameState GameStateToSet = EGameState::WaitingToStart;

    void Setup();

    void OnClickedResumeButton();
    void OnClickedOptionsButton();
    void OnClickedMenuButton();
    void OnClickedQuitButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
