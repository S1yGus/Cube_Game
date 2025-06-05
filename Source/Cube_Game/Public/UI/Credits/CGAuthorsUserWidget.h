// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGAuthorsUserWidget.generated.h"

class UButton;

UCLASS()
class CUBE_GAME_API UCGAuthorsUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UButton> Button;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnClicked();

    virtual void OnStartupAnimationFinished() override;
};
