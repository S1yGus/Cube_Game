// Cube_Game, All rights reserved.

#include "UI/Menu/CGLeaderboardUserWidget.h"
#include "CGGameModeBase.h"
#include "CGGameInstance.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Components/VerticalBox.h"
#include "UI/Menu/CGPlayerRecordRowUserWidget.h"
#include "Player/CGPlayerController.h"

void UCGLeaderboardUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGLeaderboardUserWidget::Setup()
{
    check(NameButton);
    check(ScoreButton);
    check(DateButton);
    check(BackButton);
    check(LeaderboardVerticalBox);

    NameButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedNameButton);
    ScoreButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedScoreButton);
    DateButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedDateButton);
    BackButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedBackButton);

    if (ACGGameModeBase* GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (auto* PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEsc);
    }
}

void UCGLeaderboardUserWidget::ResetWidget()
{
    BackButton->ResetButton();
}

void UCGLeaderboardUserWidget::UpdateLeaderboard()
{
    auto* GameInstnce = GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    LeaderboardVerticalBox->ClearChildren();
    for (const auto& PlayerRecord : GameInstnce->GetLeaderboard())
    {
        if (auto* PlayerRecordRowWidget = CreateWidget<UCGPlayerRecordRowUserWidget>(GetWorld(), PlayerRecordRowWidgetClass))
        {
            PlayerRecordRowWidget->SetNameText(PlayerRecord.Name);
            PlayerRecordRowWidget->SetScoreText(FText::FromString(FString::FromInt(PlayerRecord.Score)));
            PlayerRecordRowWidget->SetDateText(FText::AsDate(PlayerRecord.DateTime));
            LeaderboardVerticalBox->AddChild(PlayerRecordRowWidget);
        }
    }
}

void UCGLeaderboardUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::Leaderboard)
        return;

    ResetWidget();
    UpdateLeaderboard();
}

void UCGLeaderboardUserWidget::OnPressedEsc()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedBackButton();
}

void UCGLeaderboardUserWidget::OnClickedNameButton()
{
    if (auto* GameInstance = GetGameInstance<UCGGameInstance>())
    {
        GameInstance->SortLeaderboard(
            [this](const FPlayerRecord& Lhs, const FPlayerRecord& Rhs)
            {
                return bNameAscending ? Lhs.Name.ToString() > Rhs.Name.ToString() : Lhs.Name.ToString() < Rhs.Name.ToString();
            });

        bNameAscending = !bNameAscending;
    }

    UpdateLeaderboard();
}

void UCGLeaderboardUserWidget::OnClickedScoreButton()
{
    if (auto* GameInstance = GetGameInstance<UCGGameInstance>())
    {
        GameInstance->SortLeaderboard(
            [this](const FPlayerRecord& Lhs, const FPlayerRecord& Rhs)
            {
                return bScoreAscending ? Lhs.Score > Rhs.Score : Lhs.Score < Rhs.Score;
            });

        bScoreAscending = !bScoreAscending;
    }

    UpdateLeaderboard();
}

void UCGLeaderboardUserWidget::OnClickedDateButton()
{
    if (auto* GameInstance = GetGameInstance<UCGGameInstance>())
    {
        GameInstance->SortLeaderboard(
            [this](const FPlayerRecord& Lhs, const FPlayerRecord& Rhs)
            {
                return bDateAscending ? Lhs.DateTime > Rhs.DateTime : Lhs.DateTime < Rhs.DateTime;
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

    if (ACGGameModeBase* GameMode = GetGameModeBase())
    {
        GameMode->SetGameState(EGameState::MainMenu);
    }
}
