// Cube_Game, All rights reserved.

#include "Settings/CGGameUserSettings.h"
#include "Settings/CGSetting.h"
#include "Settings/CGIntSetting.h"
#include "Settings/CGFloatSetting.h"
#include "Settings/CGActionSetting.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Saves/CGSettingsSave.h"
#include "CGGameInstance.h"
#include "AudioDevice.h"
#include "AudioThread.h"
#include "CGUtils.h"
#include "Settings/CGSettingsConstants.h"

using namespace SettingsConstants;

#define LOCTEXT_NAMESPACE "GameUserSettings"

#define BIND_SOUND_GETTER(VolumeType)                  \
    [&]()                                              \
    {                                                  \
        return SettingsSave->SoundSettings.VolumeType; \
    }

#define BIND_SOUND_SETTER(SCName, VolumeType)              \
    [&](float NewValue)                                    \
    {                                                      \
        SetSoundClassVolume(SCName, NewValue);             \
        SettingsSave->SoundSettings.VolumeType = NewValue; \
    }

static bool operator==(const FText& Text1, const FText& Text2)
{
    return Text1.ToString() == Text2.ToString();
}

static bool operator==(const FCultureData& Data, const FString& Str)
{
    return Data.Culture == Str;
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

const FAspectRatioData& UCGGameUserSettings::GetAspectRatio() const
{
    return SettingsSave->VideoSettings.AspectRatioData;
}

bool UCGGameUserSettings::GetMusicType() const
{
    return SettingsSave->SoundSettings.bStaticMusic;
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
    {
        auto Setting = CreateIntSetting(LOCTEXT("ScreenMode_Loc", "Screen mode"), FullscreenOptions, VideoSettings);
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
    }

    {
        ResolutionSetting = CreateIntSetting(LOCTEXT("Resolution_Loc", "Resolution"), GetScreenResolutions(), VideoSettings);
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
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("VSync_Loc", "V-Sync"), VSyncOptions, VideoSettings);
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
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("FramerateLimit_Loc", "Framerate limit"), FramerateOptions, VideoSettings);
        Setting->AddGetter(
            [&]()
            {
                return GetFrameRateLimit() == 0.0f ? 0    //
                                                   : FramerateOptions.IndexOfByKey(FText::AsNumber(static_cast<int32>(GetFrameRateLimit())));
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SetFrameRateLimit(NewValue == 0 ? 0.0f    //
                                                : FCString::Atof(*FramerateOptions[NewValue].ToString()));
                ApplyNonResolutionSettings();
            });
    }

    {
        TArray<FText> AspectRatioOptions;
        for (const auto& Data : AspectRatioData)
        {
            AspectRatioOptions.Add(Data.DisplayName);
        }

        auto Setting = CreateIntSetting(LOCTEXT("AspectRatio_Loc", "Aspect ratio"), AspectRatioOptions, VideoSettings);
        Setting->AddGetter(
            [&, AspectRatioOptions]()
            {
                return AspectRatioOptions.IndexOfByKey(SettingsSave->VideoSettings.AspectRatioData.DisplayName);
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SettingsSave->VideoSettings.AspectRatioData = AspectRatioData[NewValue];
                OnAspectRatioChanged.Broadcast(AspectRatioData[NewValue]);
            });
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("GraphicsQuality_Loc", "Quality"), GraphicsQualityOptions, VideoSettings);
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
    }
}

void UCGGameUserSettings::InitSoundSettings()
{
    {
        auto Setting = CreateFloatSetting(LOCTEXT("MasterVolume_Loc", "Master"), SoundSettings);
        Setting->AddGetter(BIND_SOUND_GETTER(MasterVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCMasterName, MasterVolume));
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("UIVolume_Loc", "Interface"), SoundSettings);
        Setting->AddGetter(BIND_SOUND_GETTER(UIVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCUIName, UIVolume));
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("FXVolume_Loc", "Effects"), SoundSettings);
        Setting->AddGetter(BIND_SOUND_GETTER(FXVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCFXName, FXVolume));
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("MusicVolume_Loc", "Music"), SoundSettings);
        Setting->AddGetter(BIND_SOUND_GETTER(MusicVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCMusicName, MusicVolume));
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("MusicType_Loc", "Music type"), MusicTypeOptions, SoundSettings);
        Setting->AddGetter(
            [&]()
            {
                return static_cast<int32>(SettingsSave->SoundSettings.bStaticMusic);
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SettingsSave->SoundSettings.bStaticMusic = static_cast<bool>(NewValue);
                OnMusicTypeChanged.Broadcast(SettingsSave->SoundSettings.bStaticMusic);
            });
    }
}

void UCGGameUserSettings::InitGameSettings()
{
    {
        TArray<FText> LanguageOptions;
        for (const auto& Culture : CultureData)
        {
            LanguageOptions.Add(Culture.CultureName);
        }

        auto Setting = CreateIntSetting(LOCTEXT("Language_Loc", "Language"), LanguageOptions, GameSettings);
        Setting->AddGetter(
            []()
            {
                return CultureData.IndexOfByKey(UKismetInternationalizationLibrary::GetCurrentCulture());
            });
        Setting->AddSetter(
            [](int32 NewValue)
            {
                UKismetInternationalizationLibrary::SetCurrentCulture(CultureData[NewValue].Culture, true);
            });
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("PopUpType_Loc", "Pop-up type"), PopUpTypeOptions, GameSettings);
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
    }

    {
        auto Setting = CreateActionSetting(LOCTEXT("ResetHints_Loc", "Reset hints"), LOCTEXT("ResetHintsButton_Loc", "RESET"), GameSettings);
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
                for (const auto& HintPair : SettingsSave->GameSettings.HintsStatus.HintsMap)
                {
                    if (!HintPair.Value)    // If hint already have been shown.
                    {
                        return true;
                    }
                }

                for (const auto& HintPair : SettingsSave->GameSettings.HintsStatus.ReceivingHintsMap)
                {
                    if (!HintPair.Value)    // If hint already have been shown.
                    {
                        return true;
                    }
                }

                return false;
            });
    }

    {
        auto Setting = CreateActionSetting(LOCTEXT("ClearLeaderboard_Loc", "Clear leaderboard"), LOCTEXT("ClearLeaderboardButton_Loc", "CLEAR"), GameSettings);
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
            });
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

            for (const auto& [SoundClass, SoundClassProperties] : ADevice->GetSoundClassPropertyMap())
            {
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

UCGIntSetting* UCGGameUserSettings::CreateIntSetting(const FText& Name, const TArray<FText>& Options, TArray<UCGSetting*>& AddTo)
{
    const auto Setting = CreateSetting<UCGIntSetting>(Name, AddTo);
    Setting->SetOptions(Options);

    return Setting;
}

UCGFloatSetting* UCGGameUserSettings::CreateFloatSetting(const FText& Name, TArray<UCGSetting*>& AddTo)
{
    const auto Setting = CreateSetting<UCGFloatSetting>(Name, AddTo);

    return Setting;
}

UCGActionSetting* UCGGameUserSettings::CreateActionSetting(const FText& Name, const FText& ActionName, TArray<UCGSetting*>& AddTo)
{
    const auto Setting = CreateSetting<UCGActionSetting>(Name, AddTo);
    Setting->SetActionName(ActionName);

    return Setting;
}

#undef LOCTEXT_NAMESPACE
