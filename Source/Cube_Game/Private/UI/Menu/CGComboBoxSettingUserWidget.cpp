// Cube_Game, All rights reserved.

#include "UI/Menu/CGComboBoxSettingUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "Components/ComboBoxString.h"
#include "Settings/CGIntSetting.h"

void UCGComboBoxSettingUserWidget::Init(TObjectPtr<UCGIntSetting> NewSetting)
{
    if (!NewSetting)
        return;

    Setting = NewSetting;
    SettingNameText->SetText(NewSetting->GetName());

    Update();
}

void UCGComboBoxSettingUserWidget::Update()
{
    if (!Setting.IsValid())
        return;

    SettingComboBox->ClearOptions();
    for (const FText& Option : Setting.Get()->GetOptions())
    {
        SettingComboBox->AddOption(Option.ToString());
    }

    const int32 CurrentValue = Setting.Get()->GetCurrentValue();
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
    if (SelectionType == ESelectInfo::Direct || !Setting.IsValid())
        return;

    Setting.Get()->SetValue(SettingComboBox->FindOptionIndex(SelectedItem));
}
