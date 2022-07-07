// Cube_Game, All rights reserved.

#include "UI/Menu/CGLeaderboardUserWidget.h"
#include "CGGameModeBase.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Components/VerticalBox.h"
#include "CGGameInstance.h"
#include "UI/Menu/CGPlayerRecordRowUserWidget.h"
#include "Saves/CGLeaderboardSave.h"

void UCGLeaderboardUserWidget::ResetWidget()
{
    BackButton->ResetButton();
}

void UCGLeaderboardUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

inline const UCGLeaderboardSave* UCGLeaderboardUserWidget::GetLeaderboardSave() const
{
    const auto GameInstance = GetGameInstance<UCGGameInstance>();
    return GameInstance ? GameInstance->GetLeaderboardSave() : nullptr;
}

void UCGLeaderboardUserWidget::Setup()
{
    if (const auto GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &UCGLeaderboardUserWidget::OnGameStateChanged);
    }

    if (NameButton)
    {
        NameButton->OnClickedButton.AddUObject(this, &UCGLeaderboardUserWidget::OnClickedNameButton);
    }

    if (ScoreButton)
    {
        ScoreButton->OnClickedButton.AddUObject(this, &UCGLeaderboardUserWidget::OnClickedScoreButton);
    }

    if (DateButton)
    {
        DateButton->OnClickedButton.AddUObject(this, &UCGLeaderboardUserWidget::OnClickedDateButton);
    }

    if (BackButton)
    {
        BackButton->OnClickedButton.AddUObject(this, &UCGLeaderboardUserWidget::OnClickedBackButton);
    }
}

void UCGLeaderboardUserWidget::UpdateLeaderboard()
{
    if (!LeaderboardVerticalBox)
        return;

    LeaderboardVerticalBox->ClearChildren();

    auto LeaderboardSave = GetLeaderboardSave();
    if (!LeaderboardSave)
        return;

    for (const auto& PlayerRecord : LeaderboardSave->GetLeaderboard())
    {
        const auto PlayerRecordRowWidget = CreateWidget<UCGPlayerRecordRowUserWidget>(GetWorld(), PlayerRecordRowWidgetClass);
        if (!PlayerRecordRowWidget)
            continue;

        LeaderboardVerticalBox->AddChild(PlayerRecordRowWidget);
        PlayerRecordRowWidget->SetNameText(PlayerRecord.Name);
        PlayerRecordRowWidget->SetScoreText(FText::AsNumber(PlayerRecord.Score));
        PlayerRecordRowWidget->SetDateText(FText::AsDate(PlayerRecord.DateTime));
    }
}

void UCGLeaderboardUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::Leaderboard)
        return;

    UpdateLeaderboard();
}

void UCGLeaderboardUserWidget::OnClickedNameButton()
{
    if (const auto GameInstance = GetGameInstance<UCGGameInstance>())
    {
        GameInstance->SortLeaderboard(
            [&](const FPlayerRecord& Record1, const FPlayerRecord& Record2)
            {
                return bNameAscending ? Record1.Name.ToString() > Record2.Name.ToString() : Record1.Name.ToString() < Record2.Name.ToString();
            });

        bNameAscending = !bNameAscending;
    }

    UpdateLeaderboard();
}

void UCGLeaderboardUserWidget::OnClickedScoreButton()
{
    if (const auto GameInstance = GetGameInstance<UCGGameInstance>())
    {
        GameInstance->SortLeaderboard(
            [&](const FPlayerRecord& Record1, const FPlayerRecord& Record2)
            {
                return bScoreAscending ? Record1.Score > Record2.Score : Record1.Score < Record2.Score;
            });

        bScoreAscending = !bScoreAscending;
    }

    UpdateLeaderboard();
}

void UCGLeaderboardUserWidget::OnClickedDateButton()
{
    if (const auto GameInstance = GetGameInstance<UCGGameInstance>())
    {
        GameInstance->SortLeaderboard(
            [&](const FPlayerRecord& Record1, const FPlayerRecord& Record2)
            {
                return bDateAscending ? Record1.DateTime > Record2.DateTime : Record1.DateTime < Record2.DateTime;
            });

        bDateAscending = !bDateAscending;
    }

    UpdateLeaderboard();
}

void UCGLeaderboardUserWidget::OnClickedBackButton()
{
    ShowFadeoutAnimation();
}

void UCGLeaderboardUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    if (Animation != FadeoutAnimation)
        return;

    const auto GameMode = GetGameModeBase();
    if (!GameMode)
        return;

    GameMode->SetGameState(EGameState::MainMenu);
}
