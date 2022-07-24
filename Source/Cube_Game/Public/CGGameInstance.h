// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGCoreTypes.h"
#include "CGGameInstance.generated.h"

class UCGLeaderboardSave;

UCLASS()
class CUBE_GAME_API UCGGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    EDifficulty GetDifficulty() const { return CurrentDifficulty; }
    void SetDifficulty(EDifficulty NewDifficylty) { CurrentDifficulty = NewDifficylty; }

    const TArray<FPlayerRecord>& GetLeaderboard() const;
    void AddToLeaderboard(const FPlayerRecord& PlayerRecord);
    void SortLeaderboard(TFunction<bool(const FPlayerRecord&, const FPlayerRecord&)> Predicate);
    void ClearLeaderboard();

    void OpenMainMenu();
    void StartGame();
    void QuitGame(APlayerController* SpecificPlayer);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName MenuLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName GameLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leaderboard")
    FString LeaderboardSaveSlotName = "LeaderboardSave";

    virtual void OnStart() override;

private:
    EDifficulty CurrentDifficulty = EDifficulty::Medium;

    UPROPERTY()
    UCGLeaderboardSave* LeaderboardSave;

    void CheckLeaderboardSave();
};
