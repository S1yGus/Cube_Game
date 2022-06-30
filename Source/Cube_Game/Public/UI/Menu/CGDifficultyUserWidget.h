// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGDifficultyUserWidget.generated.h"

class UCGButtonUserWidget;

UCLASS()
class CUBE_GAME_API UCGDifficultyUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* EasyButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* MediumButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* HardButton;

    virtual void NativeOnInitialized() override;

private:
    void SetDifficulty(EDifficulty NewDifficulty);

    void OnClickedEasyButton();
    void OnClickedMediumButton();
    void OnClickedHardButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
