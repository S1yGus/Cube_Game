// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGOptionsUserWidget.generated.h"

class UVerticalBox;
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
    EGameState GameStateToSet{EGameState::WaitingToStart};

    void InitSettingsWidgets(const TArray<UCGSetting*>& SettingsArray, UVerticalBox* VerticalBox);

    void Setup();
    void ResetWidget();

    void UpdateOptions();

    void OnGameStateChanged(EGameState NewGameState);
    void OnPressedEsc();
    void OnResolutionChanged();
    void OnClickedBackButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
