// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CGCoreTypes.h"
#include <functional>
#include "CGLeaderboardSave.generated.h"

UCLASS()
class CUBE_GAME_API UCGLeaderboardSave : public USaveGame
{
    GENERATED_BODY()

public:
    const TArray<FPlayerRecord>& GetLeaderboard() const { return Leaderboard; }
    void AddToLeaderboard(const FPlayerRecord& PlayerRecord);
    void SortLeaderboard(std::function<bool(const FPlayerRecord&, const FPlayerRecord&)> Predicate);
    void ClearLeaderboard();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leaderboard")
    TArray<FPlayerRecord> Leaderboard;
};
