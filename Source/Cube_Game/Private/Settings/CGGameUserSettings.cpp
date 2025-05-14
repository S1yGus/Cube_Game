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
#include "Settings/CGSettingsConstants.h"
#include "Engine/GameEngine.h"

using namespace SettingsConstants;

DEFINE_LOG_CATEGORY_STATIC(LogCGGameUserSettings, All, All)

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

constexpr int32 MinPostProcessQualityLevel{1};

static bool operator==(const FText& Lhs, const FText& Rhs)
{
    return Lhs.ToString() == Rhs.ToString();
}

static bool operator==(const FCultureData& Data, const FString& Str)
{
    return Data.Culture == Str;
}

static void SetMSAALevel(int32 MSAASamples)
{
    if (MSAASamples != 0 && MSAASamples != 2 && MSAASamples != 4 && MSAASamples != 8)
    {
        UE_LOG(LogCGGameUserSettings, Warning, TEXT("Invalid MSAA value: %d. Use 0, 2, 4, or 8."), MSAASamples);
        return;
    }

    if (auto* MSAACount = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MSAACount")))
    {
        MSAACount->Set(MSAASamples);
    }
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

bool UCGGameUserSettings::IsFistLaunch() const
{
    return SettingsSave->GameSettings.bFirstLaunch;
}

void UCGGameUserSettings::FistLaunchDone()
{
    SettingsSave->GameSettings.bFirstLaunch = false;
    SaveSettings();
}

EPopUpType UCGGameUserSettings::GetPopUpType() const
{
    return SettingsSave->GameSettings.PopUpType;
}

bool UCGGameUserSettings::AreHintsEnabled() const
{
    return SettingsSave->GameSettings.HintSettings.bHintsEnabled;
}

const FHintsStatus& UCGGameUserSettings::GetHintsStatus() const
{
    return SettingsSave->GameSettings.HintSettings.HintsStatusMap;
}

void UCGGameUserSettings::SetHintsStatus(const FHintsStatus& NewHintsMap)
{
    SettingsSave->GameSettings.HintSettings.HintsStatusMap = NewHintsMap;
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
        auto Setting = CreateIntSetting(VideoSettingName::ScreenMode, FullscreenOptions, VideoSettings);
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
        ResolutionSetting = CreateIntSetting(VideoSettingName::Resolution, GetScreenResolutions(), VideoSettings);
        ResolutionSetting->AddGetter(
            [&]() -> int32
            {
                if (GetFullscreenMode() == EWindowMode::WindowedFullscreen)
                {
                    return INDEX_NONE;
                }
                const FIntPoint CurrentResolution = GetScreenResolution();
                const auto Option = FText::FromString(FString::Printf(TEXT("%d x %d"), CurrentResolution.X, CurrentResolution.Y));
                return ResolutionSetting->GetOptions().IndexOfByKey(Option);
            });
        ResolutionSetting->AddSetter(
            [&](int32 NewValue)
            {
                if (FString LeftS, RightS; ResolutionSetting->GetOptions()[NewValue].ToString().Split(" x ", &LeftS, &RightS))
                {
                    const FIntPoint NewResolution = FIntPoint(FCString::Atoi(*LeftS), FCString::Atoi(*RightS));
                    SetScreenResolution(NewResolution);
                    ApplyResolutionSettings(false);
                    OnResolutionChanged.Broadcast();
                }
            });
    }

    {
        auto Setting = CreateIntSetting(VideoSettingName::VSync, VSyncOptions, VideoSettings);
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
        auto Setting = CreateIntSetting(VideoSettingName::FramerateLimit, FramerateOptions, VideoSettings);
        Setting->AddGetter(
            [&]()
            {
                return GetFrameRateLimit() == 0.0f ? 0    //
                                                   : FramerateOptions.IndexOfByKey(FText::AsNumber(FMath::RoundToInt(GetFrameRateLimit())));
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
        auto Setting = CreateIntSetting(VideoSettingName::GraphicsQuality, GraphicsQualityOptions, VideoSettings);
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
        auto Setting = CreateFloatSetting(SoundSettingName::MasterVolume, SoundSettings);
        Setting->AddGetter(BIND_SOUND_GETTER(MasterVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCMasterName, MasterVolume));
    }

    {
        auto Setting = CreateFloatSetting(SoundSettingName::UIVolume, SoundSettings);
        Setting->AddGetter(BIND_SOUND_GETTER(UIVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCUIName, UIVolume));
    }

    {
        auto Setting = CreateFloatSetting(SoundSettingName::FXVolume, SoundSettings);
        Setting->AddGetter(BIND_SOUND_GETTER(FXVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCFXName, FXVolume));
    }

    {
        auto Setting = CreateFloatSetting(SoundSettingName::MusicVolume, SoundSettings);
        Setting->AddGetter(BIND_SOUND_GETTER(MusicVolume));
        Setting->AddSetter(BIND_SOUND_SETTER(SCMusicName, MusicVolume));
    }
}

void UCGGameUserSettings::InitGameSettings()
{
    {
        TArray<FText> LanguageOptions;
        for (const auto& Culture : CultureData)
        {
            LanguageOptions.Emplace(Culture.CultureName);
        }

        auto Setting = CreateIntSetting(GameSettingName::Language, LanguageOptions, GameSettings);
        Setting->AddGetter(
            [&]()
            {
                return CultureData.IndexOfByKey(UKismetInternationalizationLibrary::GetCurrentCulture());
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                if (UKismetInternationalizationLibrary::SetCurrentCulture(CultureData[NewValue].Culture, true))
                {
                    OnLanguageChanged.Broadcast();
                }
            });
    }

    {
        auto Setting = CreateIntSetting(GameSettingName::PopUpType, PopUpTypeOptions, GameSettings);
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
        auto Setting = CreateIntSetting(GameSettingName::Hints, HintOptions, GameSettings);
        Setting->AddGetter(
            [&]()
            {
                return static_cast<int32>(SettingsSave->GameSettings.HintSettings.bHintsEnabled);
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SettingsSave->GameSettings.HintSettings.bHintsEnabled = static_cast<bool>(NewValue);
                OnHintSettingsChanged.Broadcast(SettingsSave->GameSettings.HintSettings);
            });
    }

    {
        auto Setting = CreateActionSetting(GameSettingName::ResetHints, ActionName::ResetHints, GameSettings);
        Setting->AddActionFunc(
            [&]()
            {
                for (auto& HintPair : SettingsSave->GameSettings.HintSettings.HintsStatusMap)
                {
                    HintPair.Value = false;
                }
                OnHintSettingsChanged.Broadcast(SettingsSave->GameSettings.HintSettings);
            });
        Setting->AddStatusFunc(
            [&]()
            {
                for (const auto& HintPair : SettingsSave->GameSettings.HintSettings.HintsStatusMap)
                {
                    if (HintPair.Value)    // If hint already have been shown.
                    {
                        return true;
                    }
                }

                return false;
            });
    }

    {
        auto Setting = CreateActionSetting(GameSettingName::ClearLeaderboard, ActionName::ClearLeaderboard, GameSettings);
        Setting->AddActionFunc(
            []()
            {
                if (const auto* GameEngine = Cast<UGameEngine>(GEngine))
                {
                    if (auto* GameInstance = Cast<UCGGameInstance>(GameEngine->GameInstance))
                    {
                        GameInstance->ClearLeaderboard();
                    }
                }
            });
        Setting->AddStatusFunc(
            []()
            {
                if (const auto* GameEngine = Cast<UGameEngine>(GEngine))
                {
                    if (const auto* GameInstance = Cast<UCGGameInstance>(GameEngine->GameInstance))
                    {
                        return !GameInstance->GetLeaderboard().IsEmpty();
                    }
                }

                return false;
            });
    }
}

TArray<FText> UCGGameUserSettings::GetScreenResolutions() const
{
    const EWindowMode::Type ScreenMode = GetFullscreenMode();

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

void UCGGameUserSettings::SetAllVideoSettings(int32 NewQuality)
{
    switch (NewQuality)
    {
        case 0:    // Low
            SetMSAALevel(0);
            break;
        case 1:    // Medium
            SetMSAALevel(2);
            break;
        case 2:    // High
            SetMSAALevel(4);
            break;
        case 3:    // Epic
            SetMSAALevel(8);
            break;
        default:
            break;
    }

    SetViewDistanceQuality(NewQuality);
    SetAntiAliasingQuality(NewQuality);
    SetPostProcessingQuality(FMath::Max(MinPostProcessQualityLevel, NewQuality));
    SetShadowQuality(NewQuality);
    SetGlobalIlluminationQuality(NewQuality);
    SetReflectionQuality(NewQuality);
    SetTextureQuality(NewQuality);
    SetVisualEffectQuality(NewQuality);
    SetFoliageQuality(NewQuality);
    SetShadingQuality(NewQuality);
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

            FAudioDeviceHandle ADevice = GEngine->GetMainAudioDevice();
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
        RunHardwareBenchmark();
        ApplySettings(false);
    }

    check(SettingsSave);
}

TObjectPtr<UCGIntSetting> UCGGameUserSettings::CreateIntSetting(const FText& Name, const TArray<FText>& Options, TArray<TObjectPtr<UCGSetting>>& AddTo)
{
    const auto Setting = CreateSetting<UCGIntSetting>(Name, AddTo);
    Setting->SetOptions(Options);

    return Setting;
}

TObjectPtr<UCGFloatSetting> UCGGameUserSettings::CreateFloatSetting(const FText& Name, TArray<TObjectPtr<UCGSetting>>& AddTo)
{
    const auto Setting = CreateSetting<UCGFloatSetting>(Name, AddTo);

    return Setting;
}

TObjectPtr<UCGActionSetting> UCGGameUserSettings::CreateActionSetting(const FText& Name, const FText& ActionName, TArray<TObjectPtr<UCGSetting>>& AddTo)
{
    const auto Setting = CreateSetting<UCGActionSetting>(Name, AddTo);
    Setting->SetActionName(ActionName);

    return Setting;
}
