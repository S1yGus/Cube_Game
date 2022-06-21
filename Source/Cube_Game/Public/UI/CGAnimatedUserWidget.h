// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGCoreTypes.h"
#include "CGAnimatedUserWidget.generated.h"

class USoundCue;

UCLASS()
class CUBE_GAME_API UCGAnimatedUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void ShowStartupAnimation();
    void ShowFadeoutAnimation();

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* StartupAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* StartupSound;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeoutAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FadeoutSound;

    virtual void SetGameState(EGameState NewGameState);
};
