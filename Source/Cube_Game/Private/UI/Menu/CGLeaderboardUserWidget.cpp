// Cube_Game, All rights reserved.

#include "UI/Menu/CGLeaderboardUserWidget.h"
#include "CGGameModeBase.h"
#include "CGGameInstance.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Components/VerticalBox.h"
#include "UI/Menu/CGPlayerRecordRowUserWidget.h"

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

    NameButton->OnClickedButton.AddUObject(this, &UCGLeaderboardUserWidget::OnClickedNameButton);
    ScoreButton->OnClickedButton.AddUObject(this, &UCGLeaderboardUserWidget::OnClickedScoreButton);
    DateButton->OnClickedButton.AddUObject(this, &UCGLeaderboardUserWidget::OnClickedDateButton);
    BackButton->OnClickedButton.AddUObject(this, &UCGLeaderboardUserWidget::OnClickedBackButton);

    if (const auto GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &UCGLeaderboardUserWidget::OnGameStateChanged);
        GameMode->OnPressedEsc.AddUObject(this, &UCGLeaderboardUserWidget::OnPressedEsc);
    }
}

void UCGLeaderboardUserWidget::ResetWidget()
{
    BackButton->ResetButton();
}

void UCGLeaderboardUserWidget::UpdateLeaderboard()
{
    auto GameInstnce = GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    LeaderboardVerticalBox->ClearChildren();
    for (const auto& PlayerRecord : GameInstnce->GetLeaderboard())
    {
        const auto PlayerRecordRowWidget = CreateWidget<UCGPlayerRecordRowUserWidget>(GetWorld(), PlayerRecordRowWidgetClass);
        if (!PlayerRecordRowWidget)
            continue;

        PlayerRecordRowWidget->SetNameText(PlayerRecord.Name);
        PlayerRecordRowWidget->SetScoreText(FText::AsNumber(PlayerRecord.Score));
        PlayerRecordRowWidget->SetDateText(FText::AsDate(PlayerRecord.DateTime));
        LeaderboardVerticalBox->AddChild(PlayerRecordRowWidget);
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
    if (!IsVisible())
        return;

    OnClickedBackButton();
}

void UCGLeaderboardUserWidget::OnClickedNameButton()
{
    if (const auto GameInstance = GetGameInstance<UCGGameInstance>())
    {
        GameInstance->SortLeaderboard(
            [=](const FPlayerRecord& Record1, const FPlayerRecord& Record2)
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
            [=](const FPlayerRecord& Record1, const FPlayerRecord& Record2)
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
            [=](const FPlayerRecord& Record1, const FPlayerRecord& Record2)
            {
                return bDateAscending ? Record1.DateTime > Record2.DateTime : Record1.DateTime < Record2.DateTime;
            });

        bDateAscending = !bDateAscending;
    }

    UpdateLeaderboard();
}

void UCGLeaderboardUserWidget::OnClickedBackButton()
{
    if (IsAnyAnimationPlaying())
        return;

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
