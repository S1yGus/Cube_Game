// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGHowToPlayUserWidget.generated.h"

class UCGTextUserWidget;
class UTextBlock;
class UCGButtonUserWidget;

UCLASS()
class CUBE_GAME_API UCGHowToPlayUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> FadeoutHintAnimation;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> FadeInHintAnimation;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGTextUserWidget> TitleText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> HintTextBlock;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> BackButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> NextButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> PrevButton;

    virtual void NativeOnInitialized() override;

private:
    int32 CurrentHintIndex{0};

    void Setup();
    void ResetWidget();

    void ShowHint(int32 HintIndex);

    void OnGameStateChanged(EGameState NewGameState);
    void OnClickedBackButton();
    void OnClickedNextButton();
    void OnClickedPrevButton();
    void OnPressedEsc();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
