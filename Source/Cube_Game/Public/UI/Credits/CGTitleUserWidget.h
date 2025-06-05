// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGTitleUserWidget.generated.h"

class UButton;

UCLASS()
class CUBE_GAME_API UCGTitleUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> Button;

    virtual void NativeOnInitialized() override;

private:
    FORCEINLINE void OpenMainMenu();

    UFUNCTION()
    void OnClicked();

    virtual void OnStartupAnimationFinished() override;
    virtual void OnFadeoutAnimationFinished() override;
};
