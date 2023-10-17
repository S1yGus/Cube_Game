// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGHUDUserWidget.generated.h"

class UWidgetAnimation;
class USoundCue;
class ACGGameMode;

UCLASS()
class CUBE_GAME_API UCGHUDUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> LowTimeAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> LowTimeSound;

    virtual void NativeOnInitialized() override;

private:
    inline ACGGameMode* GetGameMode() const;

    void OnLowTime();
    void OnPressedEscape();
};
