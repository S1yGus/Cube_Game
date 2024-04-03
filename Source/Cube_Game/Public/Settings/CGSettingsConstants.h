// Cube_Game, All rights reserved.

#pragma once

#include "Internationalization/StringTableRegistry.h"

#define LOCTEXT_NAMESPACE "SettingsConstants"

namespace SettingsConstants
{

void InitStringTable()
{
    FStringTableRegistry::Get().UnregisterStringTable("CommonWords");
    LOCTABLE_FROMFILE_GAME("CommonWords", "CommonWordsss", "/Localization/ST_CommonWords.csv");
}

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
        LOCTABLE("CommonWords", "Disable_Loc"),    //
        LOCTABLE("CommonWords", "Enable_Loc"),     //
    };

const TArray<FText> FramerateOptions    //
    {
        LOCTEXT("FramerateUnlimited_Loc", "Unlimited"),    //
        FText::FromString("30"),                           //
        FText::FromString("60"),                           //
        FText::FromString("120"),                          //
        FText::FromString("144")                           //
    };

const TArray<FText> GraphicsQualityOptions    //
    {
        LOCTEXT("GraphicsQualityLow_Loc", "Low"),               //
        LOCTEXT("GraphicsQualityMedium_Loc", "Medium"),         //
        LOCTEXT("GraphicsQualityHigh_Loc", "High"),             //
        LOCTEXT("GraphicsQualityEpic_Loc", "Epic"),             //
        LOCTEXT("GraphicsQualityCinematic_Loc", "Cinematic")    //
    };

const TArray<FText> PopUpTypeOptions    //
    {
        LOCTEXT("PopUpTypeOff_Loc", "Off"),                  //
        LOCTEXT("PopUpTypeMultiplier_Loc", "Multiplier"),    //
        LOCTEXT("PopUpTypeAmount_Loc", "Amount")             //
    };

const TArray<FText> HintOptions    //
    {
        LOCTABLE("CommonWords", "Disable_Loc"),    //
        LOCTABLE("CommonWords", "Enable_Loc"),     //
    };

}    // namespace SettingsConstants

#undef LOCTEXT_NAMESPACE
