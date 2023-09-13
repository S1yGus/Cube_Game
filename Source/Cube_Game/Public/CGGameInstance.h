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
    const TArray<FHintData>& GetHowToPlayHints() const { return HowToPlayHints; }

    TArray<FPlayerRecord> GetLeaderboard() const;
    void AddToLeaderboard(const FPlayerRecord& PlayerRecord);
    void SortLeaderboard(TFunction<bool(const FPlayerRecord&, const FPlayerRecord&)> Predicate);
    void ClearLeaderboard();

    void OpenMainMenu();
    void StartGame();
    void QuitGame(APlayerController* SpecificPlayer);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName MenuLevelName{NAME_None};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName GameLevelName{NAME_None};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leaderboard")
    FString LeaderboardSaveSlotName{"LeaderboardSave"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TArray<FHintData> HowToPlayHints;

    virtual void OnStart() override;

private:
    EDifficulty CurrentDifficulty{EDifficulty::Medium};

    UPROPERTY()
    TObjectPtr<UCGLeaderboardSave> LeaderboardSave;

    void CheckLeaderboardSave();
};
