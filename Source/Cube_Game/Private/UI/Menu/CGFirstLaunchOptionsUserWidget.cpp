// Cube_Game, All rights reserved.

#include "UI/Menu/CGFirstLaunchOptionsUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "UI/Menu/CGComboBoxSettingUserWidget.h"
#include "UI/Menu/CGSliderSettingUserWidget.h"
#include "UI/Menu/CGButtonSettingUserWidget.h"
#include "Settings/CGGameUserSettings.h"
#include "Settings/CGIntSetting.h"
#include "Settings/CGFloatSetting.h"
#include "Settings/CGActionSetting.h"
#include "Components/VerticalBox.h"
#include "Player/CGPlayerController.h"
#include "CGUtils.h"
#include "Settings/CGSettingsConstants.h"

static const TArray<FText> FirstLaunchSettingNames{SettingsConstants::GameSettingName::Language,     //
                                                   SettingsConstants::GameSettingName::PopUpType,    //
                                                   SettingsConstants::GameSettingName::Hints};

void UCGFirstLaunchOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGFirstLaunchOptionsUserWidget::InitSettingsWidgets(const TArray<TObjectPtr<UCGSetting>>& SettingsArray)
{
    using namespace CubeGame;

    SettingsVerticalBox->ClearChildren();

    for (auto& Setting : SettingsArray)
    {
        if (FirstLaunchSettingNames.FindByPredicate(
                [&](const auto& FirstLaunchSettingName)
                {
                    return FirstLaunchSettingName.ToString() == Setting->GetName().ToString();
                }))
        {
            if (auto* IntSetting = Cast<UCGIntSetting>(Setting))
            {
                Utils::CreateAndAddSettingWidget<UCGComboBoxSettingUserWidget>(GetWorld(), ComboBoxSettingWidgetClass, IntSetting, SettingsVerticalBox);
            }
            else if (auto* FloatSetting = Cast<UCGFloatSetting>(Setting))
            {
                Utils::CreateAndAddSettingWidget<UCGSliderSettingUserWidget>(GetWorld(), SliderSettingWidgetClass, FloatSetting, SettingsVerticalBox);
            }
            else if (auto* ActionSetting = Cast<UCGActionSetting>(Setting))
            {
                Utils::CreateAndAddSettingWidget<UCGButtonSettingUserWidget>(GetWorld(), ButtonSettingWidgetClass, ActionSetting, SettingsVerticalBox);
            }
        }
    }
}

void UCGFirstLaunchOptionsUserWidget::Setup()
{
    check(SettingsVerticalBox);
    check(OkButton);
    check(ComboBoxSettingWidgetClass);
    check(SliderSettingWidgetClass);
    check(ButtonSettingWidgetClass);

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        InitSettingsWidgets(GameUserSettings->GetGameSettings());

        GameUserSettings->OnLanguageChanged.AddUObject(this, &ThisClass::OnLanguageChanged);
    }

    if (auto* PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEnter.AddUObject(this, &ThisClass::OnPressedEnter);
    }

    OkButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedOkButton);
}

void UCGFirstLaunchOptionsUserWidget::ResetWidget()
{
    UpdateSettingsWidget();
}

void UCGFirstLaunchOptionsUserWidget::UpdateSettingsWidget()
{
    for (auto* Widget : SettingsVerticalBox->GetAllChildren())
    {
        if (auto* SettingWidget = Cast<ICGSettingWidgetInterface>(Widget))
        {
            SettingWidget->Update();
        }
    }
}

void UCGFirstLaunchOptionsUserWidget::OnLanguageChanged()
{
    ResetWidget();
}

void UCGFirstLaunchOptionsUserWidget::OnClickedOkButton()
{
    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SaveSettings();
    }

    TransitionToGameState(EGameState::DifficultySelection);
}

void UCGFirstLaunchOptionsUserWidget::OnPressedEnter()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedOkButton();
}

void UCGFirstLaunchOptionsUserWidget::OnFadeoutAnimationFinished()
{
    SetGameState(GameStateToSet);
}
