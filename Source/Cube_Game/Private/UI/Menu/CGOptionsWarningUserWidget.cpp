// Cube_Game, All rights reserved.

#include "UI/Menu/CGOptionsWarningUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameModeBase.h"
#include "Settings/CGGameUserSettings.h"

FText UCGOptionsWarningUserWidget::GetCountdownTime() const
{
    return FText::AsNumber(static_cast<int32>(CountdownTime));
}

void UCGOptionsWarningUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
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

void UCGOptionsWarningUserWidget::Setup()
{
    check(SaveButton);
    check(CancelButton);

    SaveButton->OnClickedButton.AddUObject(this, &ThisClass::OnSaveSettings);
    WidgetButtons.Add(SaveButton);
    CancelButton->OnClickedButton.AddUObject(this, &ThisClass::OnCancelSettings);
    WidgetButtons.Add(CancelButton);

    if (const auto GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
        GameMode->OnPressedEnt.AddUObject(this, &ThisClass::OnPressedEnter);
        GameMode->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEsc);
    }
}

void UCGOptionsWarningUserWidget::ResetWidget()
{
    for (const auto& Button : WidgetButtons)
    {
        Button->ResetButton();
    }

    CountdownTime = CancelSettingsTime;
}

void UCGOptionsWarningUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::OptionsWarning)
        return;

    ResetWidget();
}

void UCGOptionsWarningUserWidget::OnPressedEnter()
{
    if (!IsVisible())
        return;

    OnSaveSettings();
}

void UCGOptionsWarningUserWidget::OnPressedEsc()
{
    if (!IsVisible())
        return;

    OnCancelSettings();
}

void UCGOptionsWarningUserWidget::OnSaveSettings()
{
    if (IsAnyAnimationPlaying())
        return;

    const auto GameUserSettings = UCGGameUserSettings::Get();
    if (!GameUserSettings)
        return;

    GameUserSettings->ConfirmVideoMode();

    ShowFadeoutAnimation();
}

void UCGOptionsWarningUserWidget::OnCancelSettings()
{
    if (IsAnyAnimationPlaying())
        return;

    const auto GameUserSettings = UCGGameUserSettings::Get();
    if (!GameUserSettings)
        return;

    GameUserSettings->SetLastConfirmedResolutionSettings();

    ShowFadeoutAnimation();
}

void UCGOptionsWarningUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    SetGameState(EGameState::Options);
}
