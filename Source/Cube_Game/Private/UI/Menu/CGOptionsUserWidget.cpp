// Cube_Game, All rights reserved.

#include "UI/Menu/CGOptionsUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "UI/CGHUDBase.h"
#include "CGGameModeBase.h"
#include "CGGameInstance.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/CGSettingsSave.h"

void UCGOptionsUserWidget::ResetWidget()
{
    for (const auto& Button : WidgetButtons)
    {
        Button->ResetButton();
    }

    GameStateToSet = EGameState::WaitingToStart;

    UpdateOptions();
}

void UCGOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

const UCGSettingsSave* UCGOptionsUserWidget::GetSettingsSave() const
{
    const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>();
    return GameInstance ? GameInstance->GetSettingsSave() : nullptr;
}

void UCGOptionsUserWidget::SetVideoSettings(const FVideoSettings& NewVideoSettings)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstance)
        return;

    GameInstance->SetVideoSettings(NewVideoSettings);
}

void UCGOptionsUserWidget::SetSoundSettings(const FSoundSettings& NewSoundSettings)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstance)
        return;

    GameInstance->SetSoundSettings(NewSoundSettings);
}

void UCGOptionsUserWidget::SetGameSettings(const FGameSettings& NewGameSettings)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstance)
        return;

    GameInstance->SetGameSettings(NewGameSettings);
}

void UCGOptionsUserWidget::Setup()
{
    // Video.
    if (ScreenModeComboBox)
    {
        ScreenModeComboBox->OnSelectionChanged.AddDynamic(this, &UCGOptionsUserWidget::OnSelectionChangedScreenMode);
    }

    if (ResolutionComboBox)
    {
        ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UCGOptionsUserWidget::OnSelectionChangedResolution);
    }

    if (VSyncComboBox)
    {
        VSyncComboBox->OnSelectionChanged.AddDynamic(this, &UCGOptionsUserWidget::OnSelectionChangedVSync);
    }

    if (AspectRatioComboBox)
    {
        AspectRatioComboBox->OnSelectionChanged.AddDynamic(this, &UCGOptionsUserWidget::OnSelectionChangedAspectRatio);
    }

    if (QualityComboBox)
    {
        QualityComboBox->OnSelectionChanged.AddDynamic(this, &UCGOptionsUserWidget::OnSelectionChangedQuality);
    }

    // Sound.
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UCGOptionsUserWidget::OnValueChangedMasterVolume);
        MasterVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &UCGOptionsUserWidget::OnMouseCaptureEndMasterVolume);
    }

    if (FXVolumeSlider)
    {
        FXVolumeSlider->OnValueChanged.AddDynamic(this, &UCGOptionsUserWidget::OnValueChangedFXVolume);
        FXVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &UCGOptionsUserWidget::OnMouseCaptureEndFXVolume);
    }

    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UCGOptionsUserWidget::OnValueChangedMusicVolume);
        MusicVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &UCGOptionsUserWidget::OnMouseCaptureEndMusicVolume);
    }

    // Game.
    if (PopUpComboBox)
    {
        PopUpComboBox->OnSelectionChanged.AddDynamic(this, &UCGOptionsUserWidget::OnSelectionChangedPopUp);
    }

    if (ResetHintsButton)
    {
        ResetHintsButton->OnClickedButton.AddUObject(this, &UCGOptionsUserWidget::OnClickedResetHintsButton);
        WidgetButtons.Add(ResetHintsButton);
    }

    // Back.
    if (BackButton)
    {
        BackButton->OnClickedButton.AddUObject(this, &UCGOptionsUserWidget::OnClickedBackButton);
        WidgetButtons.Add(BackButton);
    }
}

void UCGOptionsUserWidget::UpdateOptions()
{
    UpdateScreenModeComboBox();
    UpdateScreenResolutionComboBox();
    UpdateVSyncComboBox();
    UpdateAspectRatioComboBox();
    UpdateQualityComboBox();

    UpdateMasterVolumeSlider();
    UpdateFXVolumeSlider();
    UpdateMusicVolumeSlider();

    UpdatePopUpComboBox();
    UpdateResetHintsButton();
}

void UCGOptionsUserWidget::UpdateScreenModeComboBox()
{
    if (!ScreenModeComboBox || !GEngine)
        return;

    const auto ScreenMode = GEngine->GetGameUserSettings()->GetFullscreenMode();
    ScreenModeComboBox->SetSelectedIndex(static_cast<int32>(ScreenMode));
}

void UCGOptionsUserWidget::UpdateScreenResolutionComboBox()
{
    if (!ResolutionComboBox || !GEngine)
        return;

    const auto ScreenMode = GEngine->GetGameUserSettings()->GetFullscreenMode();

    TArray<FIntPoint> Resolutions;
    if (ScreenMode == EWindowMode::Windowed)
    {
        UKismetSystemLibrary::GetConvenientWindowedResolutions(Resolutions);
    }
    else
    {
        UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
    }

    ResolutionComboBox->ClearOptions();
    ResolutionComboBox->SetIsEnabled(ScreenMode != EWindowMode::WindowedFullscreen);

    for (const auto& Resolution : Resolutions)
    {
        const auto Option = FString::Printf(TEXT("%d x %d"), Resolution.X, Resolution.Y);
        ResolutionComboBox->AddOption(Option);
    }

    const auto CurrentResolution = GEngine->GetGameUserSettings()->GetScreenResolution();
    const auto Option = FString::Printf(TEXT("%d x %d"), CurrentResolution.X, CurrentResolution.Y);
    ResolutionComboBox->SetSelectedOption(Option);
}

void UCGOptionsUserWidget::UpdateVSyncComboBox()
{
    if (!VSyncComboBox || !GEngine)
        return;

    const auto VSync = GEngine->GetGameUserSettings()->IsVSyncEnabled();
    VSyncComboBox->SetSelectedIndex(static_cast<int32>(VSync));
}

void UCGOptionsUserWidget::UpdateAspectRatioComboBox()
{
}

void UCGOptionsUserWidget::UpdateQualityComboBox()
{
    const auto SettingSave = GetSettingsSave();
    if (!SettingSave || !QualityComboBox)
        return;

    const auto Quality = GetSettingsSave()->GetVideoSettings().Quality;
    QualityComboBox->SetSelectedIndex(Quality);
}

void UCGOptionsUserWidget::UpdateMasterVolumeSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave || !MasterVolumeSlider)
        return;

    MasterVolumeSlider->SetValue(SettingsSave->GetSoundSettings().MasterVolume);

    UpdateMasterVolumeText();
}

void UCGOptionsUserWidget::UpdateMasterVolumeText()
{
    if (!MasterVolumeText)
        return;

    const auto MasterVolumeStr = FString::Printf(TEXT("%.0f%%"), MasterVolumeSlider->GetNormalizedValue() * 100.0f);
    MasterVolumeText->SetText(FText::FromString(MasterVolumeStr));
}

void UCGOptionsUserWidget::UpdateFXVolumeSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave || !FXVolumeSlider)
        return;

    FXVolumeSlider->SetValue(SettingsSave->GetSoundSettings().FXVolume);

    UpdateFXVolumeText();
}

void UCGOptionsUserWidget::UpdateFXVolumeText()
{
    if (!FXVolumeText)
        return;

    const auto FXVolumeStr = FString::Printf(TEXT("%.0f%%"), FXVolumeSlider->GetNormalizedValue() * 100.0f);
    FXVolumeText->SetText(FText::FromString(FXVolumeStr));
}

void UCGOptionsUserWidget::UpdateMusicVolumeSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave || !MusicVolumeSlider)
        return;

    MusicVolumeSlider->SetValue(SettingsSave->GetSoundSettings().MusicVolume);

    UpdateMusicVolumeText();
}

void UCGOptionsUserWidget::UpdateMusicVolumeText()
{
    if (!MusicVolumeText)
        return;

    const auto MusicVolumeStr = FString::Printf(TEXT("%.0f%%"), MusicVolumeSlider->GetNormalizedValue() * 100.0f);
    MusicVolumeText->SetText(FText::FromString(MusicVolumeStr));
}

void UCGOptionsUserWidget::UpdatePopUpComboBox()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave || !PopUpComboBox)
        return;

    const auto PopUpType = SettingsSave->GetGameSettings().PopUp;
    PopUpComboBox->SetSelectedIndex(static_cast<int32>(PopUpType));
}

void UCGOptionsUserWidget::UpdateResetHintsButton()
{
    if (!ResetHintsButton)
        return;

    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    bool bCanReset = false;
    const auto GameSettings = SettingsSave->GetGameSettings();
    for (const auto& HintPair : GameSettings.Hints.HintsMap)
    {
        if (!HintPair.Value)    // If hint already have been shown.
        {
            bCanReset = true;
            break;
        }
    }

    for (const auto& HintPair : GameSettings.Hints.ReceivingHintsMap)
    {
        if (bCanReset || !HintPair.Value)    // If hint already have been shown.
        {
            bCanReset = true;
            break;
        }
    }
    
    ResetHintsButton->SetIsEnabled(bCanReset);
    ResetHintsButton->SetRenderOpacity(static_cast<float>(bCanReset));
}

void UCGOptionsUserWidget::OnSelectionChangedScreenMode(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !GEngine)
        return;

    const auto ScreenMode = ScreenModeComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetFullscreenMode(static_cast<EWindowMode::Type>(ScreenMode));
    GEngine->GetGameUserSettings()->ApplyResolutionSettings(false);

    UpdateScreenResolutionComboBox();
}

void UCGOptionsUserWidget::OnSelectionChangedResolution(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !GEngine)
        return;

    FString LeftS, RightS;
    if (SelectedItem.Split(" x ", &LeftS, &RightS))
    {
        FIntPoint NewResolution = FIntPoint(FCString::Atoi(*LeftS), FCString::Atoi(*RightS));
        GEngine->GetGameUserSettings()->SetScreenResolution(NewResolution);
        GEngine->GetGameUserSettings()->ApplyResolutionSettings(false);
    }

    GameStateToSet = EGameState::OptionsWarning;
    ShowFadeoutAnimation();
}

void UCGOptionsUserWidget::OnSelectionChangedVSync(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !GEngine)
        return;

    const auto VSync = VSyncComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetVSyncEnabled(static_cast<bool>(VSync));
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UCGOptionsUserWidget::OnSelectionChangedAspectRatio(FString SelectedItem, ESelectInfo::Type SelectionType)
{
}

void UCGOptionsUserWidget::OnSelectionChangedQuality(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !GEngine)
        return;

    const auto Quality = QualityComboBox->FindOptionIndex(SelectedItem);

    GEngine->GetGameUserSettings()->SetViewDistanceQuality(Quality);
    GEngine->GetGameUserSettings()->SetAntiAliasingQuality(Quality);
    GEngine->GetGameUserSettings()->SetPostProcessingQuality(Quality);
    GEngine->GetGameUserSettings()->SetShadowQuality(Quality);
    GEngine->GetGameUserSettings()->SetGlobalIlluminationQuality(Quality);
    GEngine->GetGameUserSettings()->SetReflectionQuality(Quality);
    GEngine->GetGameUserSettings()->SetTextureQuality(Quality);
    GEngine->GetGameUserSettings()->SetVisualEffectQuality(Quality);
    GEngine->GetGameUserSettings()->SetFoliageQuality(Quality);
    GEngine->GetGameUserSettings()->SetShadingQuality(Quality);

    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();

    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto VideoSettings = SettingsSave->GetVideoSettings();
    VideoSettings.Quality = Quality;

    SetVideoSettings(VideoSettings);
}

void UCGOptionsUserWidget::OnValueChangedMasterVolume(float Value)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstance)
        return;

    UGameplayStatics::SetSoundMixClassOverride(GetWorld(),                             //
                                               GameInstance->GetDefaultSoundMix(),     //
                                               GameInstance->GetMasterSoundClass(),    //
                                               Value,                                  //
                                               1.0f,                                   //
                                               0.0f);                                  //

    UpdateMasterVolumeText();
}

void UCGOptionsUserWidget::OnMouseCaptureEndMasterVolume()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto SoundSettings = SettingsSave->GetSoundSettings();
    SoundSettings.MasterVolume = MasterVolumeSlider->GetNormalizedValue();
    SetSoundSettings(SoundSettings);
}

void UCGOptionsUserWidget::OnValueChangedFXVolume(float Value)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstance)
        return;

    UGameplayStatics::SetSoundMixClassOverride(GetWorld(),                            //
                                               GameInstance->GetDefaultSoundMix(),    //
                                               GameInstance->GetFXSoundClass(),       //
                                               Value,                                 //
                                               1.0f,                                  //
                                               0.0f);                                 //

    UpdateFXVolumeText();
}

void UCGOptionsUserWidget::OnMouseCaptureEndFXVolume()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto SoundSettings = SettingsSave->GetSoundSettings();
    SoundSettings.FXVolume = FXVolumeSlider->GetNormalizedValue();
    SetSoundSettings(SoundSettings);
}

void UCGOptionsUserWidget::OnValueChangedMusicVolume(float Value)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstance)
        return;

    UGameplayStatics::SetSoundMixClassOverride(GetWorld(),                            //
                                               GameInstance->GetDefaultSoundMix(),    //
                                               GameInstance->GetMusicSoundClass(),    //
                                               Value,                                 //
                                               1.0f,                                  //
                                               0.0f);                                 //

    UpdateMusicVolumeText();
}

void UCGOptionsUserWidget::OnMouseCaptureEndMusicVolume()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto SoundSettings = SettingsSave->GetSoundSettings();
    SoundSettings.MusicVolume = MusicVolumeSlider->GetNormalizedValue();
    SetSoundSettings(SoundSettings);
}

void UCGOptionsUserWidget::OnSelectionChangedPopUp(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !GEngine)
        return;

    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    const auto PopUpType = PopUpComboBox->FindOptionIndex(SelectedItem);
    auto GameSettings = SettingsSave->GetGameSettings();
    GameSettings.PopUp = static_cast<EPopUpType>(PopUpType);
    SetGameSettings(GameSettings);
}

void UCGOptionsUserWidget::OnClickedResetHintsButton()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstance)
        return;

    const auto SettingSave = GameInstance->GetSettingsSave();
    if (!SettingSave)
        return;

    auto GameSettings = SettingSave->GetGameSettings();
    for (auto& HintPair : GameSettings.Hints.HintsMap)
    {
        HintPair.Value = true;
    }

    for (auto& HintPair : GameSettings.Hints.ReceivingHintsMap)
    {
        HintPair.Value = true;
    }

    GameInstance->SetGameSettings(GameSettings);

    UpdateResetHintsButton();
}

void UCGOptionsUserWidget::OnClickedBackButton()
{
    ShowFadeoutAnimation();
}

void UCGOptionsUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    if (GameStateToSet == EGameState::OptionsWarning)
    {
        SetGameState(GameStateToSet);
    }
    else
    {
        const auto OwnerController = GetOwningPlayer();
        if (!OwnerController)
            return;

        const auto HUD = OwnerController->GetHUD<ACGHUDBase>();
        if (!HUD)
            return;

        HUD->BackToRootMenu();
    }
}
