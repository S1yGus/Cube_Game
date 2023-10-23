// Cube_Game, All rights reserved.

#include "UI/Menu/CGButtonSettingUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Settings/CGActionSetting.h"

void UCGButtonSettingUserWidget::Init(TObjectPtr<UCGActionSetting> NewSetting)
{
    if (!NewSetting)
        return;

    Setting = NewSetting;
    SettingNameText->SetText(NewSetting->GetName());
    ActionButton->SetText(NewSetting->GetActionName());

    Update();
}

void UCGButtonSettingUserWidget::Update()
{
    if (!Setting.IsValid())
        return;

    ActionButton->SetIsEnabled(Setting.Get()->IsActionAvailable());
}

void UCGButtonSettingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingNameText);
    check(ActionButton);

    ActionButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedButton);
}

void UCGButtonSettingUserWidget::OnClickedButton()
{
    if (!Setting.IsValid())
        return;

    Setting.Get()->DoAction();
    Update();
}
