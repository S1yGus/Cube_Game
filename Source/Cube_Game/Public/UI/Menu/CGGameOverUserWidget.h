// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGGameOverUserWidget.generated.h"

class UCGButtonUserWidget;
class UTextBlock;

UCLASS()
class CUBE_GAME_API UCGGameOverUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ScoreTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* PlayAgainButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* MenuButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* QuitButton;

    virtual void NativeOnInitialized() override;

private:
    EGameState GameStateToSet = EGameState::WaitingToStart;

    void OnClickedPlayAgainButton();
    void OnClickedMenuButton();
    void OnClickedQuitButton();

    void OnGameStateChanged(EGameState NewGameState);
    FText FormatGameOverText(int32 Score);

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
