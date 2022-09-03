// Cube_Game, All rights reserved.

#include "UI/Menu/CGMainMenuUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameInstance.h"
#include "CGGameModeBase.h"
#include "Player/CGPlayerController.h"

void UCGMainMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGMainMenuUserWidget::Setup()
{
    check(GameButton);
    check(LeaderButton);
    check(HowButton);
    check(OptionsButton);
    check(QuitButton);

    GameButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedGameButton);
    WidgetButtons.Add(GameButton);
    LeaderButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedLeaderButton);
    WidgetButtons.Add(LeaderButton);
    HowButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedHowButton);
    WidgetButtons.Add(HowButton);
    OptionsButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedOptionsButton);
    WidgetButtons.Add(OptionsButton);
    QuitButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedQuitButton);
    WidgetButtons.Add(QuitButton);

    if (const auto GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &UCGMainMenuUserWidget::OnGameStateChanged);
    }

    if (const auto PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEsc);
    }
}

void UCGMainMenuUserWidget::ResetWidget()
{
    for (const auto& Button : WidgetButtons)
    {
        Button->ResetButton();
    }
}

void UCGMainMenuUserWidget::ChangeGameState(EGameState NewGameState)
{
    GameStateToSet = NewGameState;
    ShowFadeoutAnimation();
}

void UCGMainMenuUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::MainMenu)
        return;

    ResetWidget();
}

void UCGMainMenuUserWidget::OnPressedEsc()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedQuitButton();
}

void UCGMainMenuUserWidget::OnClickedGameButton()
{
    ChangeGameState(EGameState::DifficultySelection);
}

void UCGMainMenuUserWidget::OnClickedLeaderButton()
{
    ChangeGameState(EGameState::Leaderboard);
}

void UCGMainMenuUserWidget::OnClickedHowButton()
{
    ChangeGameState(EGameState::HowToPlay);
}

void UCGMainMenuUserWidget::OnClickedOptionsButton()
{
    ChangeGameState(EGameState::Options);
}

void UCGMainMenuUserWidget::OnClickedQuitButton()
{
    const auto GameInstnce = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    GameInstnce->QuitGame(GetOwningPlayer());
}

void UCGMainMenuUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    SetGameState(GameStateToSet);
}
