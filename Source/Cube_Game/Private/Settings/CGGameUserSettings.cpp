// Cube_Game, All rights reserved.

#include "Settings/CGGameUserSettings.h"
#include "Settings/CGSetting.h"
#include "Settings/CGIntSetting.h"
#include "Settings/CGFloatSetting.h"
#include "Settings/CGActionSetting.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/CGSettingsSave.h"
#include "CGGameInstance.h"
#include "AudioDevice.h"
#include "AudioThread.h"
#include "CGUtils.h"

#define LOCTEXT_NAMESPACE "GameUserSettings"

#define BIND_SOUND_GETTER(VolumeType)                                                                                                                                    \
    [&]()                                                                                                                                                                \
    {                                                                                                                                                                    \
        return SettingsSave->SoundSettings.VolumeType;                                                                                                                   \
    }

#define BIND_SOUND_SETTER(SCName, VolumeType)                                                                                                                            \
    [&](float NewValue)                                                                                                                                                  \
    {                                                                                                                                                                    \
        SetSoundClassVolume(SCName, NewValue);                                                                                                                           \
        SettingsSave->SoundSettings.VolumeType = NewValue;                                                                                                               \
    }

static const FString SettingsSaveSlotName = "SettingsSave";
static const FString SCMasterName = "SC_Master";
static const FString SCUIName = "SC_UI";
static const FString SCFXName = "SC_FX";
static const FString SCMusicName = "SC_Music";

static bool operator==(const FText& Text1, const FText& Text2)
{
    return Text1.ToString() == Text2.ToString();
}

UCGGameUserSettings::UCGGameUserSettings()
{
    InitVideoSettings();
    InitSoundSettings();
    InitGameSettings();
}

UCGGameUserSettings* UCGGameUserSettings::Get()
{
    return GEngine ? Cast<UCGGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

EPopUpType UCGGameUserSettings::GetPopUpType() const
{
    return SettingsSave->GameSettings.PopUpType;
}

const FHintsStatus& UCGGameUserSettings::GetHintsStatus() const
{
    return SettingsSave->GameSettings.HintsStatus;
}

void UCGGameUserSettings::SetGameplayHintsStatus(const TMap<EHintType, bool>& NewHintsMap)
{
    SettingsSave->GameSettings.HintsStatus.HintsMap = NewHintsMap;
    SaveSettings();
}

void UCGGameUserSettings::SetReceivingHintsStatus(const TMap<ECubeType, bool>& NewHintsMap)
{
    SettingsSave->GameSettings.HintsStatus.ReceivingHintsMap = NewHintsMap;
    SaveSettings();
}

void UCGGameUserSettings::SetLastConfirmedResolutionSettings()
{
    SetFullscreenMode(GetLastConfirmedFullscreenMode());
    SetScreenResolution(GetLastConfirmedScreenResolution());
    ApplyResolutionSettings(false);
    UpdateResolutionSetting();
}

void UCGGameUserSettings::InitSoundVolume()
{
    SetSoundClassVolume(SCMasterName, SettingsSave->SoundSettings.MasterVolume);
    SetSoundClassVolume(SCUIName, SettingsSave->SoundSettings.UIVolume);
    SetSoundClassVolume(SCFXName, SettingsSave->SoundSettings.FXVolume);
    SetSoundClassVolume(SCMusicName, SettingsSave->SoundSettings.MusicVolume);
}

void UCGGameUserSettings::LoadSettings(bool bForceReload)
{
    Super::LoadSettings(bForceReload);

    CheckSettingsSave();
}

void UCGGameUserSettings::SaveSettings()
{
    Super::SaveSettings();

    UGameplayStatics::SaveGameToSlot(SettingsSave, SettingsSaveSlotName, 0);
}

void UCGGameUserSettings::InitVideoSettings()
{
    TArray<FText> FullscreenOptions    //
        {
            LOCTEXT("Fullscreen_Loc", "Fullscreen"),            //
            LOCTEXT("WindowedFullscreen_Loc", "Borderless"),    //
            LOCTEXT("Windowed_Loc", "Windowed")                 //
        };

    TArray<FText> VSyncOptions    //
        {
            LOCTEXT("VSyncDisabled_Loc", "Disabled"),    //
            LOCTEXT("VSyncEnabled_Loc", "Enabled")       //
        };

    TArray<FText> FramerateOptions    //
        {
            LOCTEXT("FramerateUnlimited_Loc", "Unlimited"),    //
            FText::FromString("30"),                           //
            FText::FromString("60"),                           //
            FText::FromString("120"),                          //
            FText::FromString("144")                           //
        };

    TArray<FText> AspectRatioOptions    //
        {
            FText::FromString("16:9"),    //
            FText::FromString("21:9")     //
        };

    TArray<FText> GraphicsQualityOptions    //
        {
            LOCTEXT("GraphicsQualityLow_Loc", "Low"),               //
            LOCTEXT("GraphicsQualityMedium_Loc", "Medium"),         //
            LOCTEXT("GraphicsQualityHigh_Loc", "High"),             //
            LOCTEXT("GraphicsQualityEpic_Loc", "Epic"),             //
            LOCTEXT("GraphicsQualityCinematic_Loc", "Cinematic")    //
        };

    {
        auto Setting = NewObject<UCGIntSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("ScreenMode_Loc", "Screen mode"));
        Setting->SetOptions(FullscreenOptions);
        Setting->AddGetter(
            [&]()
            {
                return static_cast<int32>(GetFullscreenMode());
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SetFullscreenMode(static_cast<EWindowMode::Type>(NewValue));
                ApplyResolutionSettings(false);
                SetLowestResolution();
            });
        VideoSettings.Add(Setting);
    }

    {
        ResolutionSetting = NewObject<UCGIntSetting>();
        check(ResolutionSetting);
        ResolutionSetting->SetName(LOCTEXT("Resolution_Loc", "Resolution"));
        ResolutionSetting->SetOptions(GetScreenResolutions());
        ResolutionSetting->AddGetter(
            [&]() -> int32
            {
                if (GetFullscreenMode() == EWindowMode::Type::WindowedFullscreen)
                {
                    return INDEX_NONE;
                }
                const auto CurrentResolution = GetScreenResolution();
                const auto Option = FText::FromString(FString::Printf(TEXT("%d x %d"), CurrentResolution.X, CurrentResolution.Y));
                return ResolutionSetting->GetOptions().IndexOfByKey(Option);
            });
        ResolutionSetting->AddSetter(
            [&](int32 NewValue)
            {
                FString LeftS, RightS;
                if (ResolutionSetting->GetOptions()[NewValue].ToString().Split(" x ", &LeftS, &RightS))
                {
                    FIntPoint NewResolution = FIntPoint(FCString::Atoi(*LeftS), FCString::Atoi(*RightS));
                    SetScreenResolution(NewResolution);
                    ApplyResolutionSettings(false);
                    OnResolutionChanged.Broadcast();
                }
            });
        VideoSettings.Add(ResolutionSetting);
    }

    {
        auto Setting = NewObject<UCGIntSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("VSync_Loc", "V-Sync"));
        Setting->SetOptions(VSyncOptions);
        Setting->AddGetter(
            [&]()
            {
                return static_cast<int32>(IsVSyncEnabled());
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SetVSyncEnabled(static_cast<bool>(NewValue));
                ApplyNonResolutionSettings();
            });
        VideoSettings.Add(Setting);
    }

    {
        auto Setting = NewObject<UCGIntSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("FramerateLimit_Loc", "Framerate limit"));
        Setting->SetOptions(FramerateOptions);
        Setting->AddGetter(
            [&, FramerateOptions]()
            {
                return GetFrameRateLimit() == 0.0f ? 0    //
                                                   : FramerateOptions.IndexOfByKey(FText::AsNumber(static_cast<int32>(GetFrameRateLimit())));
            });
        Setting->AddSetter(
            [&, FramerateOptions](int32 NewValue)
            {
                SetFrameRateLimit(NewValue == 0 ? 0.0f    //
                                                : FCString::Atof(*FramerateOptions[NewValue].ToString()));
                ApplyNonResolutionSettings();
            });
        VideoSettings.Add(Setting);
    }

    // TODO:
    /*{
        auto Setting = NewObject<UCGIntSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("AspectRatio_Loc", "Aspect ratio"));
        Setting->SetOptions(AspectRatioOptions);
        VideoSettings.Add(Setting);
    }*/

    {
        auto Setting = NewObject<UCGIntSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("GraphicsQuality_Loc", "Quality"));
        Setting->SetOptions(GraphicsQualityOptions);
        Setting->AddGetter(
            [&]()
            {
                return GetAntiAliasingQuality();    // All video settings have the same index.
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SetAllVideoSettings(NewValue);
                ApplyNonResolutionSettings();
            });
        VideoSettings.Add(Setting);
    }
}

void UCGGameUserSettings::InitSoundSettings()
{
    {
        auto Setting = NewObject<UCGFloatSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("MasterVolume_Loc", "Master"));
        Setting->AddGetter(BIND_SOUND_GETTER(MasterVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCMasterName, MasterVolume));
        SoundSettings.Add(Setting);
    }

    {
        auto Setting = NewObject<UCGFloatSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("UIVolume_Loc", "Interface"));
        Setting->AddGetter(BIND_SOUND_GETTER(UIVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCUIName, UIVolume));
        SoundSettings.Add(Setting);
    }

    {
        auto Setting = NewObject<UCGFloatSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("FXVolume_Loc", "Effects"));
        Setting->AddGetter(BIND_SOUND_GETTER(FXVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCFXName, FXVolume));
        SoundSettings.Add(Setting);
    }

    {
        auto Setting = NewObject<UCGFloatSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("MusicVolume_Loc", "Music"));
        Setting->AddGetter(BIND_SOUND_GETTER(MusicVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCMusicName, MusicVolume));
        SoundSettings.Add(Setting);
    }
}

void UCGGameUserSettings::InitGameSettings()
{
    TArray<FText> PopUpTypeOptions    //
        {
            LOCTEXT("PopUpTypeOff_Loc", "Off"),                  //
            LOCTEXT("PopUpTypeMultiplier_Loc", "Multiplier"),    //
            LOCTEXT("PopUpTypeAmount_Loc", "Amount")             //
        };

    {
        auto Setting = NewObject<UCGIntSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("PopUpType_Loc", "Pop-up type"));
        Setting->SetOptions(PopUpTypeOptions);
        Setting->AddGetter(
            [&]()
            {
                return static_cast<int32>(SettingsSave->GameSettings.PopUpType);
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SettingsSave->GameSettings.PopUpType = static_cast<EPopUpType>(NewValue);
                OnPopUpTypeChanged.Broadcast(SettingsSave->GameSettings.PopUpType);
            });
        GameSettings.Add(Setting);
    }

    {
        auto Setting = NewObject<UCGActionSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("ResetHints_Loc", "Reset hints"));
        Setting->SetActionName(LOCTEXT("ResetHintsButton_Loc", "RESET"));
        Setting->AddActionFunc(
            [&]()
            {
                for (auto& HintPair : SettingsSave->GameSettings.HintsStatus.HintsMap)
                {
                    HintPair.Value = true;
                }

                for (auto& HintPair : SettingsSave->GameSettings.HintsStatus.ReceivingHintsMap)
                {
                    HintPair.Value = true;
                }

                OnHintsStatusChanged.Broadcast(SettingsSave->GameSettings.HintsStatus);
            });
        Setting->AddStatusFunc(
            [&]()
            {
                bool bCanReset = false;
                for (const auto& HintPair : SettingsSave->GameSettings.HintsStatus.HintsMap)
                {
                    if (!HintPair.Value)    // If hint already have been shown.
                    {
                        bCanReset = true;
                        break;
                    }
                }

                for (const auto& HintPair : SettingsSave->GameSettings.HintsStatus.ReceivingHintsMap)
                {
                    if (bCanReset || !HintPair.Value)    // If hint already have been shown.
                    {
                        bCanReset = true;
                        break;
                    }
                }

                return bCanReset;
            });
        GameSettings.Add(Setting);
    }

    {
        auto Setting = NewObject<UCGActionSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("ClearLeaderboard_Loc", "Clear leaderboard"));
        Setting->SetActionName(LOCTEXT("ClearLeaderboardButton_Loc", "CLEAR"));
        Setting->AddActionFunc(
            []()
            {
                const auto World = CGUtils::GetCurrentWorld();
                if (!World)
                    return;

                const auto GameInstance = World->GetGameInstance<UCGGameInstance>();
                if (!GameInstance)
                    return;

                GameInstance->ClearLeaderboard();
            });
        Setting->AddStatusFunc(
            []()
            {
                const auto World = CGUtils::GetCurrentWorld();
                if (!World)
                    return false;

                const auto GameInstance = World->GetGameInstance<UCGGameInstance>();
                if (!GameInstance)
                    return false;

                return !GameInstance->GetLeaderboard().IsEmpty();
                return true;
            });
        GameSettings.Add(Setting);
    }
}

const TArray<FText> UCGGameUserSettings::GetScreenResolutions() const
{
    const auto ScreenMode = GetFullscreenMode();

    TArray<FIntPoint> Resolutions;
    if (ScreenMode == EWindowMode::Windowed)
    {
        UKismetSystemLibrary::GetConvenientWindowedResolutions(Resolutions);
    }
    else
    {
        UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
    }

    TArray<FText> ResolutionsOptions;
    for (const auto& Resolution : Resolutions)
    {
        ResolutionsOptions.Add(FText::FromString(FString::Printf(TEXT("%d x %d"), Resolution.X, Resolution.Y)));
    }

    return ResolutionsOptions;
}

void UCGGameUserSettings::UpdateResolutionSetting()
{
    ResolutionSetting->SetOptions(GetScreenResolutions());
}

void UCGGameUserSettings::SetAllVideoSettings(int32 NewValue)
{
    SetViewDistanceQuality(NewValue);
    SetAntiAliasingQuality(NewValue);
    SetPostProcessingQuality(NewValue);
    SetShadowQuality(NewValue);
    SetGlobalIlluminationQuality(NewValue);
    SetReflectionQuality(NewValue);
    SetTextureQuality(NewValue);
    SetVisualEffectQuality(NewValue);
    SetFoliageQuality(NewValue);
    SetShadingQuality(NewValue);
}

void UCGGameUserSettings::SetLowestResolution()
{
    UpdateResolutionSetting();

    if (GetFullscreenMode() != EWindowMode::Type::WindowedFullscreen)
    {
        ResolutionSetting->SetValue(0);    // Set the lowest resolution in this screen mode.
    }
    else
    {
        OnResolutionChanged.Broadcast();
    }
}

void UCGGameUserSettings::SetSoundClassVolume(const FString& SoundClassName, float NewVolume)
{
    FAudioThread::RunCommandOnAudioThread(
        [=]()
        {
            if (!GEngine)
                return;

            auto ADevice = GEngine->GetMainAudioDevice();
            if (!ADevice)
                return;

            for (const auto& SoundClassPair : ADevice->GetSoundClassPropertyMap())
            {
                auto SoundClass = SoundClassPair.Key;

                if (SoundClass && SoundClass->GetFullName().Find(SoundClassName) != INDEX_NONE)
                {
                    SoundClass->Properties.Volume = NewVolume;
                }
            }
        });
}

void UCGGameUserSettings::CheckSettingsSave()
{
    if (SettingsSave)
        return;

    if (UGameplayStatics::DoesSaveGameExist(SettingsSaveSlotName, 0))
    {
        SettingsSave = Cast<UCGSettingsSave>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlotName, 0));
    }
    else
    {
        SettingsSave = Cast<UCGSettingsSave>(UGameplayStatics::CreateSaveGameObject(UCGSettingsSave::StaticClass()));
    }

    check(SettingsSave);
}

#undef LOCTEXT_NAMESPACE