// Cube_Game, All rights reserved.

#include "CGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/CGLeaderboardSave.h"
#include "Settings/CGGameUserSettings.h"

TArray<FPlayerRecord> UCGGameInstance::GetLeaderboard() const
{
    if (LeaderboardSave)
    {
        return LeaderboardSave->GetLeaderboard();
    }

    return {};
}

void UCGGameInstance::AddToLeaderboard(const FPlayerRecord& PlayerRecord)
{
    if (LeaderboardSave)
    {
        LeaderboardSave->AddToLeaderboard(PlayerRecord);
        UGameplayStatics::SaveGameToSlot(LeaderboardSave, LeaderboardSaveSlotName, 0);
    }
}

void UCGGameInstance::SortLeaderboard(TFunction<bool(const FPlayerRecord&, const FPlayerRecord&)> Predicate)
{
    if (LeaderboardSave)
    {
        LeaderboardSave->SortLeaderboard(Predicate);
    }
}

void UCGGameInstance::ClearLeaderboard()
{
    if (LeaderboardSave)
    {
        LeaderboardSave->ClearLeaderboard();
        UGameplayStatics::SaveGameToSlot(LeaderboardSave, LeaderboardSaveSlotName, 0);
    }
}

void UCGGameInstance::OpenMainMenu()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MenuLevel);
}

void UCGGameInstance::StartGame()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameLevel);
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
}
