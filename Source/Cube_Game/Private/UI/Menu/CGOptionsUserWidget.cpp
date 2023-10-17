// Cube_Game, All rights reserved.

#include "UI/Menu/CGOptionsUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Components/VerticalBox.h"
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

#define UPDATE_SETTINGS_WIDGETS(ContainerWidget)                           \
    for (const auto& Widget : ContainerWidget->GetAllChildren())           \
    {                                                                      \
        if (auto* SettingWidget = Cast<ICGSettingWidgetInterface>(Widget)) \
        {                                                                  \
            SettingWidget->Update();                                       \
        }                                                                  \
    }

#define CREATE_AND_ADD_SETTING_WIDGET(T, WidgetClass, Setting, ToContainerWidget) \
    T* SettingWidget = CreateWidget<T>(GetWorld(), WidgetClass);                  \
    check(SettingWidget);                                                         \
    SettingWidget->Init(Setting);                                                 \
    ToContainerWidget->AddChild(SettingWidget);

void UCGOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGOptionsUserWidget::InitSettingsWidgets(const TArray<UCGSetting*>& SettingsArray, UVerticalBox* VerticalBox)
{
    VerticalBox->ClearChildren();

    for (auto& Setting : SettingsArray)
    {
        if (auto* IntSetting = Cast<UCGIntSetting>(Setting))
        {
            CREATE_AND_ADD_SETTING_WIDGET(UCGComboBoxSettingUserWidget, ComboBoxSettingWidgetClass, IntSetting, VerticalBox);
        }
        else if (auto* FloatSetting = Cast<UCGFloatSetting>(Setting))
        {
            CREATE_AND_ADD_SETTING_WIDGET(UCGSliderSettingUserWidget, SliderSettingWidgetClass, FloatSetting, VerticalBox);
        }
        else if (auto* ActionSetting = Cast<UCGActionSetting>(Setting))
        {
            CREATE_AND_ADD_SETTING_WIDGET(UCGButtonSettingUserWidget, ButtonSettingWidgetClass, ActionSetting, VerticalBox);
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
    UPDATE_SETTINGS_WIDGETS(VideoSettingsVerticalBox);
    UPDATE_SETTINGS_WIDGETS(SoundSettingsVerticalBox);
    UPDATE_SETTINGS_WIDGETS(GameSettingsVerticalBox);
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
    GameStateToSet = EGameState::OptionsWarning;
    ShowFadeoutAnimation();
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

    if (Animation != FadeoutAnimation)
        return;

    if (GameStateToSet == EGameState::OptionsWarning)
    {
        SetGameState(GameStateToSet);
    }
    else
    {
        const APlayerController* PC = GetOwningPlayer();
        if (!PC)
            return;

        auto* HUD = PC->GetHUD<ACGHUDBase>();
        if (!HUD)
            return;

        HUD->BackToRootMenu();
    }
}
