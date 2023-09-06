// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/CGSettingWidgetInterface.h"
#include "CGSliderSettingUserWidget.generated.h"

class UCGTextUserWidget;
class USlider;
class UCGFloatSetting;

UCLASS()
class CUBE_GAME_API UCGSliderSettingUserWidget : public UUserWidget, public ICGSettingWidgetInterface
{
    GENERATED_BODY()

public:
    void Init(TObjectPtr<UCGFloatSetting> NewSetting);
    void UpdatePercentText();
    virtual void Update() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGTextUserWidget> SettingNameText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<USlider> SettingSlider;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGTextUserWidget> PercentText;

    UPROPERTY()
    TObjectPtr<UCGFloatSetting> Setting;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnValueChanged(float Value);
    UFUNCTION()
    void OnMouseCaptureEnd();
};
