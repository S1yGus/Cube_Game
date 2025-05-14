// Cube_Game, All rights reserved.

#include "UI/Menu/CGOptionsWarningUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameModeBase.h"
#include "Settings/CGGameUserSettings.h"
#include "Player/CGPlayerController.h"

FText UCGOptionsWarningUserWidget::GetCountdownTime() const
{
    return FText::AsNumber(FMath::RoundToInt(CountdownTime));
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

    if (ACGGameModeBase* GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (auto* PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEnter.AddUObject(this, &ThisClass::OnPressedEnter);
        PC->OnPressedEscape.AddUObject(this, &ThisClass::OnPressedEscape);
    }
}

void UCGOptionsWarningUserWidget::ResetWidget()
{
    for (auto& Button : WidgetButtons)
    {
        Button->ResetButton();
    }

    CountdownTime = CancelSettingsTime;
}

void UCGOptionsWarningUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState == EGameState::OptionsWarning)
    {
        ResetWidget();
    }
}

void UCGOptionsWarningUserWidget::OnPressedEnter()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnSaveSettings();
}

void UCGOptionsWarningUserWidget::OnPressedEscape()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnCancelSettings();
}

void UCGOptionsWarningUserWidget::OnSaveSettings()
{
    auto* GameUserSettings = UCGGameUserSettings::Get();
    if (!GameUserSettings)
        return;

    GameUserSettings->ConfirmVideoMode();

    ShowFadeoutAnimation();
}

void UCGOptionsWarningUserWidget::OnCancelSettings()
{
    auto* GameUserSettings = UCGGameUserSettings::Get();
    if (!GameUserSettings)
        return;

    GameUserSettings->SetLastConfirmedResolutionSettings();

    ShowFadeoutAnimation();
}

void UCGOptionsWarningUserWidget::OnFadeoutAnimationFinished()
{
    SetGameState(EGameState::Options);
}
