// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGDifficultyUserWidget.generated.h"

class UCGButtonUserWidget;
class USoundMix;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundMix* FadeOutSoundMix;

    virtual void NativeOnInitialized() override;

private:
    inline void SetDifficulty(EDifficulty NewDifficulty);

    void OnClickedEasyButton();
    void OnClickedMediumButton();
    void OnClickedHardButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
