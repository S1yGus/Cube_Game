// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGCoreTypes.h"
#include <functional>
#include "CGGameInstance.generated.h"

class USoundMix;
class USoundClass;
class UCGSettingsSave;
class UCGLeaderboardSave;

UCLASS()
class CUBE_GAME_API UCGGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    FOnAspectRatioChangedSignature OnAspectRatioChanged;

    EDifficulty GetDifficulty() const { return CurrentDifficulty; }
    void SetDifficulty(EDifficulty NewDifficylty) { CurrentDifficulty = NewDifficylty; }

    const UCGSettingsSave* GetSettingsSave() const { return SettingsSave; }
    void SetVideoSettings(const FVideoSettings& NewVideoSettings);
    void SetSoundSettings(const FSoundSettings& NewSoundSettings);
    void SetGameSettings(const FGameSettings& NewGameSettings);

    const UCGLeaderboardSave* GetLeaderboardSave() const { return LeaderboardSave; }
    void AddToLeaderboard(const FPlayerRecord& PlayerRecord);
    void SortLeaderboard(std::function<bool(const FPlayerRecord&, const FPlayerRecord&)> Predicate);
    void ClearLeaderboard();

    USoundMix* GetDefaultSoundMix() const { return DefaultSoundMix; }
    USoundClass* GetMasterSoundClass() const { return MasterSoundClass; }
    USoundClass* GetUISoundClass() const { return UISoundClass; }
    USoundClass* GetFXSoundClass() const { return FXSoundClass; }
    USoundClass* GetMusicSoundClass() const { return MusicSoundClass; }

    void OpenMainMenu();
    void StartGame();
    void QuitGame(APlayerController* SpecificPlayer);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName MenuLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName GameLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FString SettingsSaveSlotName = "SettingsSave";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundMix* DefaultSoundMix;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundClass* MasterSoundClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundClass* UISoundClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundClass* FXSoundClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundClass* MusicSoundClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leaderboard")
    FString LeaderboardSaveSlotName = "LeaderboardSave";

    virtual void OnStart() override;

private:
    EDifficulty CurrentDifficulty = EDifficulty::Medium;

    UPROPERTY()
    UCGSettingsSave* SettingsSave;
    UPROPERTY()
    UCGLeaderboardSave* LeaderboardSave;

    void CheckSettingsSave();
    void ApplyVideoSettings();
    void ApplySoundSettings();
    void ApplyGameSettings();

    void CheckLeaderboardSave();
};
