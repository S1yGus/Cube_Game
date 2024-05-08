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
    const TMap<EHintType, FHintData>& GetHintsMap() const { return HintsMap; }
    TArray<FHintData> GetHints() const;
    void FormatHints(const FText& MoveLeftKeyName, const FText& MoveRightKeyName, const FText& UseCurrentBonusKeyName);

    TArray<FPlayerRecord> GetLeaderboard() const;
    void AddToLeaderboard(const FPlayerRecord& PlayerRecord);
    void SortLeaderboard(TFunction<bool(const FPlayerRecord&, const FPlayerRecord&)> Predicate);
    void ClearLeaderboard();

    void OpenMainMenu();
    void StartGame();
    void QuitGame(APlayerController* SpecificPlayer);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    TSoftObjectPtr<UWorld> MenuLevel;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    TSoftObjectPtr<UWorld> GameLevel;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FString LeaderboardSaveSlotName{"LeaderboardSave"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<EHintType, FHintData> HintsMap;

    virtual void OnStart() override;

private:
    UPROPERTY()
    TObjectPtr<UCGLeaderboardSave> LeaderboardSave;
    bool bHintsFormatted{false};

    void CheckLeaderboardSave();
};
