// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "Interfaces/CGWidgetInterface.h"
#include "CGCoreTypes.h"
#include "CGOptionsUserWidget.generated.h"

class UCGButtonUserWidget;
class UComboBoxString;
class USlider;
class UTextBlock;
class UCGSettingsSave;

UCLASS()
class CUBE_GAME_API UCGOptionsUserWidget : public UCGAnimatedUserWidget, public ICGWidgetInterface
{
    GENERATED_BODY()

public:
    virtual void ResetWidget() override;

protected:
    // Video.
    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* ScreenModeComboBox;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* VSyncComboBox;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* AspectRatioComboBox;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* QualityComboBox;

    // Sound.
    UPROPERTY(Meta = (BindWidget))
    USlider* MasterVolumeSlider;

    UPROPERTY(Meta = (BindWidget))
    USlider* FXVolumeSlider;

    UPROPERTY(Meta = (BindWidget))
    USlider* MusicVolumeSlider;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* MasterVolumeTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* FXVolumeTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* MusicVolumeTextBlock;

    // Game.
    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* PopUpComboBox;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* ResetHintsButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* BackButton;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<UCGButtonUserWidget*> WidgetButtons;

    EGameState GameStateToSet = EGameState::WaitingToStart;

    const UCGSettingsSave* GetSettingsSave() const;
    void SetVideoSettings(const FVideoSettings& NewVideoSettings);
    void SetSoundSettings(const FSoundSettings& NewSoundSettings);
    void SetGameSettings(const FGameSettings& NewGameSettings);

    void Setup();

    void UpdateOptions();

    // Video.
    void UpdateScreenModeComboBox();
    void UpdateScreenResolutionComboBox();
    void UpdateVSyncComboBox();
    void UpdateAspectRatioComboBox();
    void UpdateQualityComboBox();

    // Sound.
    void UpdateMasterVolumeSlider();
    void UpdateMasterVolumeText();
    void UpdateFXVolumeSlider();
    void UpdateFXVolumeText();
    void UpdateMusicVolumeSlider();
    void UpdateMusicVolumeText();

    // Game.
    void UpdatePopUpComboBox();

    UFUNCTION()
    void OnSelectionChangedScreenMode(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedResolution(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedVSync(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedAspectRatio(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedQuality(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnValueChangedMasterVolume(float Value);

    UFUNCTION()
    void OnMouseCaptureEndMasterVolume();

    UFUNCTION()
    void OnValueChangedFXVolume(float Value);

    UFUNCTION()
    void OnMouseCaptureEndFXVolume();

    UFUNCTION()
    void OnValueChangedMusicVolume(float Value);

    UFUNCTION()
    void OnMouseCaptureEndMusicVolume();

    UFUNCTION()
    void OnSelectionChangedPopUp(FString SelectedItem, ESelectInfo::Type SelectionType);

    void OnClickedResetHintsButton();

    void OnClickedBackButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
