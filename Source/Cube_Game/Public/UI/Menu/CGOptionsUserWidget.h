// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "Components/VerticalBox.h"
#include "CGOptionsUserWidget.generated.h"

class UCGButtonUserWidget;
class UCGComboBoxSettingUserWidget;
class UCGSliderSettingUserWidget;
class UCGButtonSettingUserWidget;
class UCGSetting;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UCGComboBoxSettingUserWidget> ComboBoxSettingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UCGSliderSettingUserWidget> SliderSettingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UCGButtonSettingUserWidget> ButtonSettingWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    void InitSettingsWidgets(const TArray<TObjectPtr<UCGSetting>>& SettingsArray, UVerticalBox* VerticalBox);

    void Setup();
    void ResetWidget();

    void UpdateOptions();
    FORCEINLINE void UpdateSettingsWidget(UVerticalBox* Container);

    void OnGameStateChanged(EGameState NewGameState);
    void OnPressedEscape();
    void OnResolutionChanged();
    void OnClickedBackButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

    template <class T1, class T2>
    void CreateAndAddSettingWidget(TSubclassOf<UUserWidget> WidgetClass, T2* Setting, UVerticalBox* ToContainerWidget)
    {
        T1* SettingWidget = CreateWidget<T1>(GetWorld(), WidgetClass);
        check(SettingWidget);
        SettingWidget->Init(Setting);
        ToContainerWidget->AddChild(SettingWidget);
    }
};
