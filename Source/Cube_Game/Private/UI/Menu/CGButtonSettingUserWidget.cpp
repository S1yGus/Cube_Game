// Cube_Game, All rights reserved.

#include "UI/Menu/CGButtonSettingUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Settings/CGActionSetting.h"

void UCGButtonSettingUserWidget::Init(UCGActionSetting* NewSetting)
{
    if (!NewSetting)
        return;

    Setting = NewSetting;
    SettingNameText->SetText(Setting->GetName());
    ActionButton->SetText(Setting->GetActionName());

    Update();
}

void UCGButtonSettingUserWidget::Update()
{
    if (!Setting)
        return;

    ActionButton->SetIsEnabled(Setting->IsActionAvailable());
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
    if (!Setting)
        return;

    Setting->DoAction();
    Update();
}
