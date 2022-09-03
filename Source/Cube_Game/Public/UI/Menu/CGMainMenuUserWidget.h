// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGMainMenuUserWidget.generated.h"

class UCGButtonUserWidget;

UCLASS()
class CUBE_GAME_API UCGMainMenuUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* GameButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* LeaderButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* HowButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* OptionsButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* QuitButton;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<UCGButtonUserWidget*> WidgetButtons;

    EGameState GameStateToSet = EGameState::WaitingToStart;

    void Setup();
    void ResetWidget();

    inline void ChangeGameState(EGameState NewGameState);

    void OnGameStateChanged(EGameState NewGameState);
    void OnPressedEsc();
    void OnClickedGameButton();
    void OnClickedLeaderButton();
    void OnClickedHowButton();
    void OnClickedOptionsButton();
    void OnClickedQuitButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
