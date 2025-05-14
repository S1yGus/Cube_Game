// Cube_Game, All rights reserved.

#include "UI/Menu/CGOptionsUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "UI/Menu/CGComboBoxSettingUserWidget.h"
#include "UI/Menu/CGSliderSettingUserWidget.h"
#include "UI/Menu/CGButtonSettingUserWidget.h"
#include "UI/CGHUDBase.h"
#include "CGGameModeBase.h"
#include "Settings/CGGameUserSettings.h"
#include "Settings/CGIntSetting.h"
#include "Settings/CGFloatSetting.h"
#include "Settings/CGActionSetting.h"
#include "Interfaces/CGSettingWidgetInterface.h"
#include "Player/CGPlayerController.h"
#include "Components/VerticalBox.h"
#include "CGUtils.h"

void UCGOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGOptionsUserWidget::InitSettingsWidgets(const TArray<TObjectPtr<UCGSetting>>& SettingsArray, UVerticalBox* VerticalBox)
{
    using namespace CubeGame;

    VerticalBox->ClearChildren();

    for (auto& Setting : SettingsArray)
    {
        if (auto* IntSetting = Cast<UCGIntSetting>(Setting))
        {
            Utils::CreateAndAddSettingWidget<UCGComboBoxSettingUserWidget>(GetWorld(), ComboBoxSettingWidgetClass, IntSetting, VerticalBox);
        }
        else if (auto* FloatSetting = Cast<UCGFloatSetting>(Setting))
        {
            Utils::CreateAndAddSettingWidget<UCGSliderSettingUserWidget>(GetWorld(), SliderSettingWidgetClass, FloatSetting, VerticalBox);
        }
        else if (auto* ActionSetting = Cast<UCGActionSetting>(Setting))
        {
            Utils::CreateAndAddSettingWidget<UCGButtonSettingUserWidget>(GetWorld(), ButtonSettingWidgetClass, ActionSetting, VerticalBox);
        }
    }
}

void UCGOptionsUserWidget::Setup()
{
    check(VideoSettingsVerticalBox);
    check(SoundSettingsVerticalBox);
    check(GameSettingsVerticalBox);
    check(BackButton);
    check(ComboBoxSettingWidgetClass);
    check(SliderSettingWidgetClass);
    check(ButtonSettingWidgetClass);

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        InitSettingsWidgets(GameUserSettings->GetVideoSettings(), VideoSettingsVerticalBox);
        InitSettingsWidgets(GameUserSettings->GetSoundSettings(), SoundSettingsVerticalBox);
        InitSettingsWidgets(GameUserSettings->GetGameSettings(), GameSettingsVerticalBox);

        GameUserSettings->OnResolutionChanged.AddUObject(this, &ThisClass::OnResolutionChanged);
        GameUserSettings->OnLanguageChanged.AddUObject(this, &ThisClass::OnLanguageChanged);
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
    if (NewGameState == EGameState::Options)
    {
        ResetWidget();
    }
}

void UCGOptionsUserWidget::OnResolutionChanged()
{
    TransitionToGameState(EGameState::OptionsWarning);
}

void UCGOptionsUserWidget::OnLanguageChanged()
{
    ResetWidget();
}

void UCGOptionsUserWidget::OnClickedBackButton()
{
    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SaveSettings();
    }

    ShowFadeoutAnimation();
}

void UCGOptionsUserWidget::OnPressedEscape()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedBackButton();
}

void UCGOptionsUserWidget::OnFadeoutAnimationFinished()
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
