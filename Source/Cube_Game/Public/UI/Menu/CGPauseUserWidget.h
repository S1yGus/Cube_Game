// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGPauseUserWidget.generated.h"

class UCGButtonUserWidget;

UCLASS()
class CUBE_GAME_API UCGPauseUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> ResumeButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> HowButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> OptionsButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> MenuButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> QuitButton;

    virtual void NativeOnInitialized() override;
    virtual void SetGameState(EGameState NewGameState) override;

private:
    UPROPERTY()
    TArray<TObjectPtr<UCGButtonUserWidget>> WidgetButtons;

    EGameState GameStateToSet = EGameState::WaitingToStart;

    void Setup();
    void ResetWidget();

    void ChangeGameState(EGameState NewGameState);

    void OnGameStateChanged(EGameState NewGameState);
    void OnPressedEscape();
    void OnClickedResumeButton();
    void OnClickedHowButton();
    void OnClickedOptionsButton();
    void OnClickedMenuButton();
    void OnClickedQuitButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
