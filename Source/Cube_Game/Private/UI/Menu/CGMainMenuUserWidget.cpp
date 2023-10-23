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

    GameButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedGameButton);
    WidgetButtons.Add(GameButton);
    LeaderButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedLeaderButton);
    WidgetButtons.Add(LeaderButton);
    HowButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedHowButton);
    WidgetButtons.Add(HowButton);
    OptionsButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedOptionsButton);
    WidgetButtons.Add(OptionsButton);
    QuitButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedQuitButton);
    WidgetButtons.Add(QuitButton);

    if (ACGGameModeBase* GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (auto* PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEscape.AddUObject(this, &ThisClass::OnPressedEscape);
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

void UCGMainMenuUserWidget::OnPressedEscape()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedQuitButton();
}

void UCGMainMenuUserWidget::OnClickedGameButton()
{
    ShowFadeoutAnimationAndSetGameState(EGameState::DifficultySelection);
}

void UCGMainMenuUserWidget::OnClickedLeaderButton()
{
    ShowFadeoutAnimationAndSetGameState(EGameState::Leaderboard);
}

void UCGMainMenuUserWidget::OnClickedHowButton()
{
    ShowFadeoutAnimationAndSetGameState(EGameState::HowToPlay);
}

void UCGMainMenuUserWidget::OnClickedOptionsButton()
{
    ShowFadeoutAnimationAndSetGameState(EGameState::Options);
}

void UCGMainMenuUserWidget::OnClickedQuitButton()
{
    if (auto* GameInstnce = GetWorld() ? GetWorld()->GetGameInstance<UCGGameInstance>() : nullptr)
    {
        GameInstnce->QuitGame(GetOwningPlayer());
    }
}

void UCGMainMenuUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == FadeoutAnimation)
    {
        SetGameState(GameStateToSet);
    }
}
