// Cube_Game, All rights reserved.

#include "UI/Menu/CGMainMenuUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameInstance.h"
#include "CGGameModeBase.h"

void UCGMainMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGMainMenuUserWidget::Setup()
{
    check(GameButton);
    check(LeaderButton);
    check(OptionsButton);
    check(QuitButton);

    GameButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedGameButton);
    WidgetButtons.Add(GameButton);
    LeaderButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedLeaderButton);
    WidgetButtons.Add(LeaderButton);
    OptionsButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedOptionsButton);
    WidgetButtons.Add(OptionsButton);
    QuitButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedQuitButton);
    WidgetButtons.Add(QuitButton);

    if (const auto GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &UCGMainMenuUserWidget::OnGameStateChanged);
        GameMode->OnPressedEsc.AddUObject(this, &UCGMainMenuUserWidget::OnPressedEsc);
    }
}

void UCGMainMenuUserWidget::ResetWidget()
{
    for (const auto& Button : WidgetButtons)
    {
        Button->ResetButton();
    }
}

void UCGMainMenuUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::MainMenu)
        return;

    ResetWidget();
}

void UCGMainMenuUserWidget::OnPressedEsc()
{
    if (!IsVisible())
        return;

    OnClickedQuitButton();
}

void UCGMainMenuUserWidget::OnClickedGameButton()
{
    if (IsAnyAnimationPlaying())
        return;

    GameStateToSet = EGameState::DifficultySelection;
    ShowFadeoutAnimation();
}

void UCGMainMenuUserWidget::OnClickedLeaderButton()
{
    if (IsAnyAnimationPlaying())
        return;

    GameStateToSet = EGameState::Leaderboard;
    ShowFadeoutAnimation();
}

void UCGMainMenuUserWidget::OnClickedOptionsButton()
{
    if (IsAnyAnimationPlaying())
        return;

    GameStateToSet = EGameState::Options;
    ShowFadeoutAnimation();
}

void UCGMainMenuUserWidget::OnClickedQuitButton()
{
    if (IsAnyAnimationPlaying())
        return;

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
