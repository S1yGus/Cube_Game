// Cube_Game, All rights reserved.

#include "UI/Menu/CGOptionsUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Settings/CGGameUserSettings.h"
#include "UI/Menu/CGComboBoxSettingUserWidget.h"
#include "UI/Menu/CGSliderSettingUserWidget.h"
#include "UI/Menu/CGButtonSettingUserWidget.h"
#include "CGGameModeBase.h"
#include "UI/CGHUDBase.h"
#include "Settings/CGIntSetting.h"
#include "Settings/CGFloatSetting.h"
#include "Settings/CGActionSetting.h"
#include "Interfaces/CGSettingWidgetInterface.h"
#include "Player/CGPlayerController.h"

void UCGOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGOptionsUserWidget::InitSettingsWidgets(const TArray<TObjectPtr<UCGSetting>>& SettingsArray, UVerticalBox* VerticalBox)
{
    VerticalBox->ClearChildren();

    for (auto& Setting : SettingsArray)
    {
        if (auto* IntSetting = Cast<UCGIntSetting>(Setting))
        {
            CreateAndAddSettingWidget<UCGComboBoxSettingUserWidget>(ComboBoxSettingWidgetClass, IntSetting, VerticalBox);
        }
        else if (auto* FloatSetting = Cast<UCGFloatSetting>(Setting))
        {
            CreateAndAddSettingWidget<UCGSliderSettingUserWidget>(SliderSettingWidgetClass, FloatSetting, VerticalBox);
        }
        else if (auto* ActionSetting = Cast<UCGActionSetting>(Setting))
        {
            CreateAndAddSettingWidget<UCGButtonSettingUserWidget>(ButtonSettingWidgetClass, ActionSetting, VerticalBox);
        }
    }
}

void UCGOptionsUserWidget::Setup()
{
    check(VideoSettingsVerticalBox);
    check(SoundSettingsVerticalBox);
    check(GameSettingsVerticalBox);
    check(BackButton);

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->OnResolutionChanged.AddUObject(this, &ThisClass::OnResolutionChanged);

        InitSettingsWidgets(GameUserSettings->GetVideoSettings(), VideoSettingsVerticalBox);
        InitSettingsWidgets(GameUserSettings->GetSoundSettings(), SoundSettingsVerticalBox);
        InitSettingsWidgets(GameUserSettings->GetGameSettings(), GameSettingsVerticalBox);
    }

    BackButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedBackButton);

    if (ACGGameModeBase* GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (auto* PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEscape.AddUObject(this, &ThisClass::OnPressedEscape);
    }
}

void UCGOptionsUserWidget::ResetWidget()
{
    BackButton->ResetButton();
    GameStateToSet = EGameState::WaitingToStart;
    UpdateOptions();
}

void UCGOptionsUserWidget::UpdateOptions()
{
    UpdateSettingsWidget(VideoSettingsVerticalBox);
    UpdateSettingsWidget(SoundSettingsVerticalBox);
    UpdateSettingsWidget(GameSettingsVerticalBox);
}

void UCGOptionsUserWidget::UpdateSettingsWidget(UVerticalBox* Container)
{
    for (auto* Widget : Container->GetAllChildren())
    {
        if (auto* SettingWidget = Cast<ICGSettingWidgetInterface>(Widget))
        {
            SettingWidget->Update();
        }
    }
}

void UCGOptionsUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::Options)
        return;

    ResetWidget();
}

void UCGOptionsUserWidget::OnPressedEscape()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedBackButton();
}

void UCGOptionsUserWidget::OnResolutionChanged()
{
    ShowFadeoutAnimationAndSetGameState(EGameState::OptionsWarning);
}

void UCGOptionsUserWidget::OnClickedBackButton()
{
    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SaveSettings();
    }

    ShowFadeoutAnimation();
}

void UCGOptionsUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == FadeoutAnimation)
    {
        if (GameStateToSet == EGameState::OptionsWarning)
        {
            SetGameState(GameStateToSet);
        }
        else if (auto* HUD = GetOwningPlayer() ? GetOwningPlayer()->GetHUD<ACGHUDBase>() : nullptr)
        {
            HUD->BackToRootMenu();
        }
    }
}
