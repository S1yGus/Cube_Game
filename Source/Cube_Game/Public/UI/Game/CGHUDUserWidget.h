// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGHUDUserWidget.generated.h"

class UWidgetAnimation;
class USoundCue;

UCLASS()
class CUBE_GAME_API UCGHUDUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* LowTimeAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* LowTimeSound;

    virtual void NativeOnInitialized() override;

private:
    void OnLowTime();
};
