// Cube_Game, All rights reserved.

#include "CGGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/CGSettingsSave.h"
#include "Saves/CGLeaderboardSave.h"
#include "GameFramework/GameUserSettings.h"

void UCGGameInstance::SetVideoSettings(const FVideoSettings& NewVideoSettings)
{
    if (!SettingsSave)
        return;

    SettingsSave->SetVideoSettings(NewVideoSettings);

    UGameplayStatics::SaveGameToSlot(SettingsSave, SettingsSaveSlotName, 0);
}

void UCGGameInstance::SetSoundSettings(const FSoundSettings& NewSoundSettings)
{
    if (!SettingsSave)
        return;

    SettingsSave->SetSoundSettings(NewSoundSettings);

    UGameplayStatics::SaveGameToSlot(SettingsSave, SettingsSaveSlotName, 0);
}

void UCGGameInstance::SetGameSettings(const FGameSettings& NewGameSettings)
{
    if (!SettingsSave)
        return;

    SettingsSave->SetGameSettings(NewGameSettings);

    UGameplayStatics::SaveGameToSlot(SettingsSave, SettingsSaveSlotName, 0);
}

void UCGGameInstance::AddToLeaderboard(const FPlayerRecord& PlayerRecord)
{
    if (!LeaderboardSave)
        return;

    LeaderboardSave->AddToLeaderboard(PlayerRecord);
    UGameplayStatics::SaveGameToSlot(LeaderboardSave, LeaderboardSaveSlotName, 0);
}

void UCGGameInstance::SortLeaderboard(std::function<bool(const FPlayerRecord&, const FPlayerRecord&)> Predicate)
{
    if (!LeaderboardSave)
        return;

    LeaderboardSave->SortLeaderboard(Predicate);
}

void UCGGameInstance::ClearLeaderboard()
{
    if (!LeaderboardSave)
        return;

    LeaderboardSave->ClearLeaderboard();
    UGameplayStatics::SaveGameToSlot(LeaderboardSave, LeaderboardSaveSlotName, 0);
}

void UCGGameInstance::OpenMainMenu()
{
    if (MenuLevelName == NAME_None)
        return;

    UGameplayStatics::OpenLevel(this, MenuLevelName);
}

void UCGGameInstance::StartGame()
{
    if (GameLevelName == NAME_None)
        return;

    UGameplayStatics::OpenLevel(this, GameLevelName);
}

void UCGGameInstance::QuitGame(APlayerController* SpecificPlayer)
{
    UKismetSystemLibrary::QuitGame(this, SpecificPlayer, EQuitPreference::Quit, true);
}

void UCGGameInstance::OnStart()
{
    Super::OnStart();

    CheckSettingsSave();
    CheckLeaderboardSave();
}

void UCGGameInstance::CheckSettingsSave()
{
    if (UGameplayStatics::DoesSaveGameExist(SettingsSaveSlotName, 0))
    {
        SettingsSave = Cast<UCGSettingsSave>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlotName, 0));
    }
    else
    {
        SettingsSave = Cast<UCGSettingsSave>(UGameplayStatics::CreateSaveGameObject(UCGSettingsSave::StaticClass()));
    }

    if (SettingsSave)
    {
        ApplyVideoSettings();
        ApplySoundSettings();
        ApplyGameSettings();
    }
}

void UCGGameInstance::ApplyVideoSettings()
{
}

void UCGGameInstance::ApplySoundSettings()
{
    if (!DefaultSoundMix)
        return;

    const auto SoundSettings = SettingsSave->GetSoundSettings();
    if (MasterSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, MasterSoundClass, SoundSettings.MasterVolume, 1.0f, 0.0f);
    }

    if (UISoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, UISoundClass, SoundSettings.UIVolume, 1.0f, 0.0f);
    }

    if (FXSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, FXSoundClass, SoundSettings.FXVolume, 1.0f, 0.0f);
    }

    if (MusicSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, MusicSoundClass, SoundSettings.MusicVolume, 1.0f, 0.0f);
    }
}

void UCGGameInstance::ApplyGameSettings()
{
}

void UCGGameInstance::CheckLeaderboardSave()
{
    if (UGameplayStatics::DoesSaveGameExist(LeaderboardSaveSlotName, 0))
    {
        LeaderboardSave = Cast<UCGLeaderboardSave>(UGameplayStatics::LoadGameFromSlot(LeaderboardSaveSlotName, 0));
    }
    else
    {
        LeaderboardSave = Cast<UCGLeaderboardSave>(UGameplayStatics::CreateSaveGameObject(UCGLeaderboardSave::StaticClass()));
    }
}
