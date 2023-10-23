// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGGameOverUserWidget.generated.h"

class UCGButtonUserWidget;
class UCGTextUserWidget;
class UEditableText;

UCLASS()
class CUBE_GAME_API UCGGameOverUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> AddAnimation;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGTextUserWidget> GameOverText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> AddButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> PlayAgainButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> MenuButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> QuitButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UEditableText> NameEditableText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Text Format", Meta = (ToolTip = "To specify the number of score, use the argument {0}."))
    FText GameOverFormat;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Text Format")
    int32 PlayerNameMinLen = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Text Format")
    int32 PlayerNameMaxLen = 9;

    virtual void NativeOnInitialized() override;

private:
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
