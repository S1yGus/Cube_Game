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
    void Init(UCGActionSetting* NewSetting);
    virtual void Update() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    UCGTextUserWidget* SettingNameText;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* ActionButton;

    UPROPERTY()
    UCGActionSetting* Setting;

    virtual void NativeOnInitialized() override;

private:
    void OnClickedButton();
};
