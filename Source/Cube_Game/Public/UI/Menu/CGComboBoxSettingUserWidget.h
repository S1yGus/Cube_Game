// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/CGSettingWidgetInterface.h"
#include "CGComboBoxSettingUserWidget.generated.h"

class UCGTextUserWidget;
class UComboBoxString;
class UCGIntSetting;

UCLASS()
class CUBE_GAME_API UCGComboBoxSettingUserWidget : public UUserWidget, public ICGSettingWidgetInterface
{
    GENERATED_BODY()

public:
    void Init(UCGIntSetting* NewSetting);
    virtual void Update() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    UCGTextUserWidget* SettingNameText;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* SettingComboBox;

    UPROPERTY()
    UCGIntSetting* Setting;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
