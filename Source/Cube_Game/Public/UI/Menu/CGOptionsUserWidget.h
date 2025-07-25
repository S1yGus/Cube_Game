// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGOptionsUserWidget.generated.h"

class UCGButtonUserWidget;
class UCGComboBoxSettingUserWidget;
class UCGSliderSettingUserWidget;
class UCGButtonSettingUserWidget;
class UCGSetting;
class UVerticalBox;

UCLASS()
class CUBE_GAME_API UCGOptionsUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UVerticalBox> VideoSettingsVerticalBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UVerticalBox> SoundSettingsVerticalBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UVerticalBox> GameSettingsVerticalBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> BackButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UCGComboBoxSettingUserWidget> ComboBoxSettingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UCGSliderSettingUserWidget> SliderSettingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UCGButtonSettingUserWidget> ButtonSettingWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    void InitSettingsWidgets(const TArray<TObjectPtr<UCGSetting>>& SettingsArray, UVerticalBox* VerticalBox);
    void Setup();
    void ResetWidget();
    void UpdateOptions();
    FORCEINLINE void UpdateSettingsWidget(UVerticalBox* Container);

    void OnGameStateChanged(EGameState NewGameState);
    void OnResolutionChanged();
    void OnLanguageChanged();
    void OnClickedBackButton();
    void OnPressedEscape();

    virtual void OnFadeoutAnimationFinished() override;
};
