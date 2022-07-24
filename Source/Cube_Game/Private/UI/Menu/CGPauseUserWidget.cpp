// Cube_Game, All rights reserved.

#include "UI/Menu/CGPauseUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameModeBase.h"
#include "CGGameInstance.h"
#include "Player/CGPlayerController.h"

void UCGPauseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGPauseUserWidget::Setup()
{
    check(ResumeButton);
    check(OptionsButton);
    check(MenuButton);
    check(QuitButton);

    ResumeButton->OnClickedButton.AddUObject(this, &UCGPauseUserWidget::OnClickedResumeButton);
    WidgetButtons.Add(ResumeButton);
    OptionsButton->OnClickedButton.AddUObject(this, &UCGPauseUserWidget::OnClickedOptionsButton);
    WidgetButtons.Add(OptionsButton);
    MenuButton->OnClickedButton.AddUObject(this, &UCGPauseUserWidget::OnClickedMenuButton);
    WidgetButtons.Add(MenuButton);
    QuitButton->OnClickedButton.AddUObject(this, &UCGPauseUserWidget::OnClickedQuitButton);
    WidgetButtons.Add(QuitButton);

    if (const auto GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &UCGPauseUserWidget::OnGameStateChanged);
        GameMode->OnPressedEsc.AddUObject(this, &UCGPauseUserWidget::OnPressedEsc);
    }
}

void UCGPauseUserWidget::ResetWidget()
{
    for (const auto& Button : WidgetButtons)
    {
        Button->ResetButton();
    }

    GameStateToSet = EGameState::WaitingToStart;
}

void UCGPauseUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::Pause)
        return;

    ResetWidget();
}

void UCGPauseUserWidget::OnPressedEsc()
{
    if (!IsVisible())
        return;

    OnClickedResumeButton();
}

void UCGPauseUserWidget::OnClickedResumeButton()
{
    if (IsAnyAnimationPlaying())
        return;

    GameStateToSet = EGameState::Game;
    ShowFadeoutAnimation();
}

void UCGPauseUserWidget::OnClickedOptionsButton()
{
    if (IsAnyAnimationPlaying())
        return;

    GameStateToSet = EGameState::Options;
    ShowFadeoutAnimation();
}

void UCGPauseUserWidget::OnClickedMenuButton()
{
    if (IsAnyAnimationPlaying())
        return;

    GameStateToSet = EGameState::MainMenu;
    ShowFadeoutAnimation();
}

void UCGPauseUserWidget::OnClickedQuitButton()
{
    if (IsAnyAnimationPlaying())
        return;

    const auto GameInstnce = GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    GameInstnce->QuitGame(GetOwningPlayer());
}

void UCGPauseUserWidget::SetGameState(EGameState NewGameState)
{
    const auto GameModeBase = GetGameModeBase();
    if (!GameModeBase)
        return;

    GameModeBase->SetGameState(NewGameState);

    if (NewGameState == EGameState::Game)
    {
        GameModeBase->ClearPause();
    }
}

void UCGPauseUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    if (GameStateToSet == EGameState::MainMenu)
    {
        if (const auto GameInstnce = GetGameInstance<UCGGameInstance>())
        {
            GameInstnce->OpenMainMenu();
        }
    }
    else
    {
        SetGameState(GameStateToSet);
    }
}
