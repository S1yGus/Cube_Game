// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/CGSettingWidgetInterface.h"
#include "CGButtonSettingUserWidget.generated.h"

class UCGTextUserWidget;
class UCGButtonUserWidget;
class UCGActionSetting;

UCLASS()
class CUBE_GAME_API UCGButtonSettingUserWidget : public UUserWidget, public ICGSettingWidgetInterface
{
    GENERATED_BODY()

public:
    void Init(TObjectPtr<UCGActionSetting> NewSetting);
    virtual void Update() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGTextUserWidget> SettingNameText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> ActionButton;

    virtual void NativeOnInitialized() override;

private:
    TWeakObjectPtr<UCGActionSetting> Setting;

    void OnClickedButton();
};
