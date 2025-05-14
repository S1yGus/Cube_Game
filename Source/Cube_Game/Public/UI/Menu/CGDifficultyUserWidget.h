// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGDifficultyUserWidget.generated.h"

class UVerticalBox;
class UCGButtonUserWidget;
class USoundMix;

UCLASS()
class CUBE_GAME_API UCGDifficultyUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UVerticalBox> DifficultyButtonsVerticalBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UCGButtonUserWidget> DifficultyButtonWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundMix> FadeOutSoundMix;

    virtual void NativeOnInitialized() override;

private:
    void InitDifficultyButtons();
    void CreateAndAddDifficultyButton(EDifficulty Difficulty);

    virtual void OnFadeoutAnimationFinished() override;
};
