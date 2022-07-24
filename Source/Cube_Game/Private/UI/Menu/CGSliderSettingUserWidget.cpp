// Cube_Game, All rights reserved.

#include "UI/Menu/CGSliderSettingUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "Components/Slider.h"
#include "Settings/CGFloatSetting.h"

void UCGSliderSettingUserWidget::Init(UCGFloatSetting* NewSetting)
{
    if (!NewSetting)
        return;

    Setting = NewSetting;
    SettingNameText->SetText(Setting->GetName());

    Update();
}

void UCGSliderSettingUserWidget::UpdatePercentText()
{
    const auto PercentStr = FString::Printf(TEXT("%.0f%%"), SettingSlider->GetNormalizedValue() * 100.0f);    // * 100.0f for 100%.
    PercentText->SetText(FText::FromString(PercentStr));
}

void UCGSliderSettingUserWidget::Update()
{
    if (!Setting)
        return;

    SettingSlider->SetValue(Setting->GetCurrentValue());

    UpdatePercentText();
}

void UCGSliderSettingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingNameText);
    check(SettingSlider);
    check(PercentText);

    SettingSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnValueChanged);
    SettingSlider->OnMouseCaptureEnd.AddDynamic(this, &ThisClass::OnMouseCaptureEnd);
}

void UCGSliderSettingUserWidget::OnValueChanged(float Value)
{
    UpdatePercentText();
}

void UCGSliderSettingUserWidget::OnMouseCaptureEnd()
{
    if (!Setting)
        return;

    Setting->SetValue(SettingSlider->GetNormalizedValue());
}
