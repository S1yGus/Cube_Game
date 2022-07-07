// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGGameOverUserWidget.generated.h"

class UCGButtonUserWidget;
class UCGTextUserWidget;
class ACGGameMode;
class UEditableText;

UCLASS()
class CUBE_GAME_API UCGGameOverUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* AddAnimation;

    UPROPERTY(Meta = (BindWidget))
    UCGTextUserWidget* GameOverText;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* AddButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* PlayAgainButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* MenuButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* QuitButton;

    UPROPERTY(Meta = (BindWidget))
    UEditableText* NameEditableText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Text Format", Meta = (ToolTip = "To specify the number of score, use the argument {0}"))
    FString GameOverFormatStr = "You have scored {0} points.";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Text Format")
    int32 PlayerNameMinLen = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Text Format")
    int32 PlayerNameMaxLen = 9;

    virtual void NativeOnInitialized() override;

private:
    EGameState GameStateToSet = EGameState::WaitingToStart;

    void Setup();

    void UpdateAddButton();

    void OnClickedAddButton();
    void OnClickedPlayAgainButton();
    void OnClickedMenuButton();
    void OnClickedQuitButton();
    void OnGameStateChanged(EGameState NewGameState);
    UFUNCTION()
    void OnNameTextChanged(const FText& NewText);

    FText FormatGameOverText(int32 Score);

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
