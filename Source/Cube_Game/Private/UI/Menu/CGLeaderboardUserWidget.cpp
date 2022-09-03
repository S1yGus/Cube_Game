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

    if (const auto GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (const auto PC = GetOwningPlayer<ACGPlayerController>())
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
        PlayerRecordRowWidget->SetScoreText(FText::FromString(FString::FromInt(PlayerRecord.Score)));
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
    if (!IsVisible() || IsAnyAnimationPlaying())
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
