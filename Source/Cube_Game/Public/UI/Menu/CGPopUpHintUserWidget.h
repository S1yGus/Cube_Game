// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "Interfaces/CGWidgetInterface.h"
#include "CGCoreTypes.h"
#include "CGPopUpHintUserWidget.generated.h"

class UTextBlock;
class UCGButtonUserWidget;
class ACGGameMode;

UCLASS()
class CUBE_GAME_API UCGPopUpHintUserWidget : public UCGAnimatedUserWidget, public ICGWidgetInterface
{
    GENERATED_BODY()

public:
    virtual void ResetWidget() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* TitleTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* HintTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* ResumeButton;

    virtual void NativeOnInitialized() override;

private:
    void OnShowPopUpHint(const FPopUpHint& PopUpHint);
    void OnClickedResumeButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
