// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGPopUpHintUserWidget.generated.h"

class UCGTextUserWidget;
class UTextBlock;
class UCGButtonUserWidget;

UCLASS()
class CUBE_GAME_API UCGPopUpHintUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGTextUserWidget> TitleText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> HintTextBlock;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> ResumeButton;

    virtual void NativeOnInitialized() override;

private:
    void Setup();
    void ResetWidget();

    void OnGameStateChanged(EGameState NewGameState);
    void OnPressedEnter();
    void OnShowPopUpHint(const FHintData& HintData);
    void OnClickedResumeButton();

    virtual void OnFadeoutAnimationFinished() override;
};
