// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGFirstLaunchOptionsUserWidget.generated.h"

class UCGButtonUserWidget;
class UCGSetting;
class UCGComboBoxSettingUserWidget;
class UCGSliderSettingUserWidget;
class UCGButtonSettingUserWidget;
class UVerticalBox;

UCLASS()
class CUBE_GAME_API UCGFirstLaunchOptionsUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UVerticalBox> SettingsVerticalBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> OkButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UCGComboBoxSettingUserWidget> ComboBoxSettingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UCGSliderSettingUserWidget> SliderSettingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UCGButtonSettingUserWidget> ButtonSettingWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    void InitSettingsWidgets(const TArray<TObjectPtr<UCGSetting>>& SettingsArray);
    void Setup();
    void ResetWidget();
    void UpdateSettingsWidget();

    void OnLanguageChanged();
    void OnClickedOkButton();
    void OnPressedEnter();
};
