// Cube_Game, All rights reserved.

#pragma once

#include "Internationalization/StringTableRegistry.h"
#include "CGCoreTypes.h"

#define LOCTEXT_NAMESPACE "SettingsConstants"

namespace SettingsConstants
{

struct VideoSettingName
{
    VideoSettingName() = delete;

    static FText ScreenMode;
    static FText Resolution;
    static FText VSync;
    static FText FramerateLimit;
    static FText GraphicsQuality;
};

struct SoundSettingName
{
    SoundSettingName() = delete;

    static FText MasterVolume;
    static FText UIVolume;
    static FText FXVolume;
    static FText MusicVolume;
};

struct GameSettingName
{
    GameSettingName() = delete;

    static FText Language;
    static FText PopUpType;
    static FText Hints;
    static FText ResetHints;
    static FText ClearLeaderboard;
};

struct ActionName
{
    ActionName() = delete;

    static FText ResetHints;
    static FText ClearLeaderboard;
};

// clang-format off
const FString SettingsSaveSlotName = "SettingsSave";

const FString SCMasterName         = "SC_Master";
const FString SCUIName             = "SC_UI";
const FString SCFXName             = "SC_FX";
const FString SCMusicName          = "SC_Music";

const TArray<FCultureData> CultureData =
    {
        {"en",     LOCTEXT("English_Loc", "English")},
        {"ru-RU",  LOCTEXT("Russian_Loc", "Russian")}
    };
// clang-format on

const TArray<FText> FullscreenOptions    //
    {
        LOCTEXT("Fullscreen_Loc", "Fullscreen"),            //
        LOCTEXT("WindowedFullscreen_Loc", "Borderless"),    //
        LOCTEXT("Windowed_Loc", "Windowed")                 //
    };

const TArray<FText> VSyncOptions    //
    {
        LOCTEXT("Disable_Loc", "Disable"),    //
        LOCTEXT("Enable_Loc", "Enable")       //
    };

const TArray<FText> FramerateOptions    //
    {
        LOCTEXT("FramerateUnlimited_Loc", "Unlimited"),    //
        FText::FromString("30"),                           //
        FText::FromString("60"),                           //
        FText::FromString("120"),                          //
        FText::FromString("144"),                          //
        FText::FromString("175")                           //
    };

const TArray<FText> GraphicsQualityOptions    //
    {
        LOCTEXT("GraphicsQualityLow_Loc", "Low"),          //
        LOCTEXT("GraphicsQualityMedium_Loc", "Medium"),    //
        LOCTEXT("GraphicsQualityHigh_Loc", "High"),        //
        LOCTEXT("GraphicsQualityEpic_Loc", "Epic"),        //
    };

const TArray<FText> PopUpTypeOptions    //
    {
        LOCTEXT("PopUpTypeOff_Loc", "Off"),                  //
        LOCTEXT("PopUpTypeMultiplier_Loc", "Multiplier"),    //
        LOCTEXT("PopUpTypeAmount_Loc", "Amount")             //
    };

const TArray<FText> HintOptions    //
    {
        LOCTEXT("Disable_Loc", "Disable"),    //
        LOCTEXT("Enable_Loc", "Enable")       //
    };

}    // namespace SettingsConstants

#undef LOCTEXT_NAMESPACE
