// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "Interfaces/CGWidgetInterface.h"
#include "CGLeaderboardUserWidget.generated.h"

class UVerticalBox;
class UCGButtonUserWidget;
class UCGPlayerRecordRowUserWidget;
class UCGLeaderboardSave;

UCLASS()
class CUBE_GAME_API UCGLeaderboardUserWidget : public UCGAnimatedUserWidget, public ICGWidgetInterface
{
    GENERATED_BODY()

public:
    virtual void ResetWidget() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    UVerticalBox* LeaderboardVerticalBox;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* NameButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* ScoreButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* DateButton;

    UPROPERTY(Meta = (BindWidget))
    UCGButtonUserWidget* BackButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UCGPlayerRecordRowUserWidget> PlayerRecordRowWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    bool bNameAscending = true;
    bool bScoreAscending = true;
    bool bDateAscending = true;

    inline const UCGLeaderboardSave* GetLeaderboardSave() const;

    void Setup();

    void UpdateLeaderboard();

    void OnGameStateChanged(EGameState NewGameState);
    void OnClickedNameButton();
    void OnClickedScoreButton();
    void OnClickedDateButton();
    void OnClickedBackButton();
    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
