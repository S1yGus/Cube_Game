// Cube_Game, All rights reserved.

#include "Settings/CGSettingsConstants.h"

#define LOCTEXT_NAMESPACE "SettingsConstants"

namespace SettingsConstants
{

FText VideoSettingName::ScreenMode = LOCTEXT("ScreenMode_Loc", "Screen mode");
FText VideoSettingName::Resolution = LOCTEXT("Resolution_Loc", "Resolution");
FText VideoSettingName::VSync = LOCTEXT("VSync_Loc", "V-Sync");
FText VideoSettingName::FramerateLimit = LOCTEXT("FramerateLimit_Loc", "Framerate limit");
FText VideoSettingName::GraphicsQuality = LOCTEXT("GraphicsQuality_Loc", "Quality");

FText SoundSettingName::MasterVolume = LOCTEXT("MasterVolume_Loc", "Master");
FText SoundSettingName::UIVolume = LOCTEXT("UIVolume_Loc", "Interface");
FText SoundSettingName::FXVolume = LOCTEXT("FXVolume_Loc", "Effects");
FText SoundSettingName::MusicVolume = LOCTEXT("MusicVolume_Loc", "Music");

FText GameSettingName::Language = LOCTEXT("Language_Loc", "Language");
FText GameSettingName::PopUpType = LOCTEXT("PopUpType_Loc", "Pop-up type");
FText GameSettingName::Hints = LOCTEXT("Hints_Loc", "Hints");
FText GameSettingName::ResetHints = LOCTEXT("ResetHints_Loc", "Reset hints");
FText GameSettingName::ClearLeaderboard = LOCTEXT("ClearLeaderboard_Loc", "Clear leaderboard");

FText ActionName::ResetHints = LOCTEXT("ResetHintsButton_Loc", "RESET");
FText ActionName::ClearLeaderboard = LOCTEXT("ClearLeaderboardButton_Loc", "CLEAR");

}    // namespace SettingsConstants

#undef LOCTEXT_NAMESPACE
