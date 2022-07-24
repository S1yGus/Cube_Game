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
    void Init(UCGFloatSetting* NewSetting);
    void UpdatePercentText();
    virtual void Update() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    UCGTextUserWidget* SettingNameText;

    UPROPERTY(Meta = (BindWidget))
    USlider* SettingSlider;

    UPROPERTY(Meta = (BindWidget))
    UCGTextUserWidget* PercentText;

    UPROPERTY()
    UCGFloatSetting* Setting;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnValueChanged(float Value);
    UFUNCTION()
    void OnMouseCaptureEnd();
};
