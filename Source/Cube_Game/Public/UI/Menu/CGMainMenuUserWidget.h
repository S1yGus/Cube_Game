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
    TObjectPtr<UCGButtonUserWidget> GameButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> LeaderButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> HowButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> OptionsButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> QuitButton;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<TObjectPtr<UCGButtonUserWidget>> WidgetButtons;

    EGameState GameStateToSet = EGameState::WaitingToStart;

    void Setup();
    void ResetWidget();

    inline void ChangeGameState(EGameState NewGameState);

    void OnGameStateChanged(EGameState NewGameState);
    void OnPressedEscape();
    void OnClickedGameButton();
    void OnClickedLeaderButton();
    void OnClickedHowButton();
    void OnClickedOptionsButton();
    void OnClickedQuitButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
