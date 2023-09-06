// Cube_Game, All rights reserved.

#include "CGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/CGLeaderboardSave.h"
#include "Settings/CGGameUserSettings.h"

const TArray<FPlayerRecord>& UCGGameInstance::GetLeaderboard() const
{
    return LeaderboardSave->GetLeaderboard();
}

void UCGGameInstance::AddToLeaderboard(const FPlayerRecord& PlayerRecord)
{
    LeaderboardSave->AddToLeaderboard(PlayerRecord);
    UGameplayStatics::SaveGameToSlot(LeaderboardSave, LeaderboardSaveSlotName, 0);
}

void UCGGameInstance::SortLeaderboard(TFunction<bool(const FPlayerRecord&, const FPlayerRecord&)> Predicate)
{
    LeaderboardSave->SortLeaderboard(Predicate);
}

void UCGGameInstance::ClearLeaderboard()
{
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

    CheckLeaderboardSave();

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->InitSoundVolume();
    }
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

    check(LeaderboardSave);
}
