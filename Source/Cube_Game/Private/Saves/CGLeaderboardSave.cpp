// Cube_Game, All rights reserved.

#include "Saves/CGLeaderboardSave.h"

void UCGLeaderboardSave::AddToLeaderboard(const FPlayerRecord& PlayerRecord)
{
    Leaderboard.Add(PlayerRecord);
}

void UCGLeaderboardSave::SortLeaderboard(TFunction<bool(const FPlayerRecord&, const FPlayerRecord&)> Predicate)
{
    Leaderboard.Sort(Predicate);
}

void UCGLeaderboardSave::ClearLeaderboard()
{
    Leaderboard.Empty();
}
