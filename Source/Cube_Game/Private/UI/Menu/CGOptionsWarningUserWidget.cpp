// Cube_Game, All rights reserved.

#include "UI/Menu/CGOptionsWarningUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameModeBase.h"
#include "GameFramework/GameUserSettings.h"

void UCGOptionsWarningUserWidget::ResetWidget()
{
    for (const auto& Button : WidgetButtons)
    {
        Button->ResetButton();
    }

    CountdownTime = CancelSettingsTime;
}

FText UCGOptionsWarningUserWidget::GetCountdownTime() const
{
    return FText::AsNumber(static_cast<int32>(CountdownTime));
}

void UCGOptionsWarningUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SaveButton)
    {
        SaveButton->OnClickedButton.AddUObject(this, &UCGOptionsWarningUserWidget::OnSaveSettings);
        WidgetButtons.Add(SaveButton);
    }

    if (CancelButton)
    {
        CancelButton->OnClickedButton.AddUObject(this, &UCGOptionsWarningUserWidget::OnCancelSettings);
        WidgetButtons.Add(CancelButton);
    }
}

void UCGOptionsWarningUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    CountdownTime -= InDeltaTime;
    if (CountdownTime <= 0.0f && !IsAnimationPlaying(FadeoutAnimation))
    {
        OnCancelSettings();
    }
}

void UCGOptionsWarningUserWidget::OnSaveSettings()
{
    GEngine->GetGameUserSettings()->SaveSettings();

    ShowFadeoutAnimation();
}

void UCGOptionsWarningUserWidget::OnCancelSettings()
{
    GEngine->GetGameUserSettings()->LoadSettings();
    GEngine->GetGameUserSettings()->ApplyResolutionSettings(false);

    ShowFadeoutAnimation();
}

void UCGOptionsWarningUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    SetGameState(EGameState::Options);
}
