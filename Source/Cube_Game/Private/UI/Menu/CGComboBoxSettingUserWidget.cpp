// Cube_Game, All rights reserved.

#include "UI/Menu/CGComboBoxSettingUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "Components/ComboBoxString.h"
#include "Settings/CGIntSetting.h"

void UCGComboBoxSettingUserWidget::Init(UCGIntSetting* NewSetting)
{
    if (!NewSetting)
        return;

    Setting = NewSetting;
    SettingNameText->SetText(Setting->GetName());

    Update();
}

void UCGComboBoxSettingUserWidget::Update()
{
    if (!Setting)
        return;

    SettingComboBox->ClearOptions();
    for (const auto& Option : Setting->GetOptions())
    {
        SettingComboBox->AddOption(Option.ToString());
    }

    const auto CurrentValue = Setting->GetCurrentValue();
    if (CurrentValue != INDEX_NONE)
    {
        SettingComboBox->SetSelectedIndex(CurrentValue);
    }

    SettingComboBox->SetIsEnabled(CurrentValue != INDEX_NONE);
}

void UCGComboBoxSettingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingNameText);
    check(SettingComboBox);

    SettingComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnSelectionChanged);
}

void UCGComboBoxSettingUserWidget::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !Setting)
        return;

    Setting->SetValue(SettingComboBox->FindOptionIndex(SelectedItem));
}
