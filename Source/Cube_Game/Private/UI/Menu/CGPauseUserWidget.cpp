// Cube_Game, All rights reserved.

#include "UI/Menu/CGPauseUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameModeBase.h"
#include "CGGameInstance.h"

void UCGPauseUserWidget::ResetWidget()
{
    for (const auto& Button : WidgetButtons)
    {
        Button->ResetButton();
    }

    GameStateToSet = EGameState::WaitingToStart;
}

void UCGPauseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGPauseUserWidget::Setup()
{
    if (ResumeButton)
    {
        ResumeButton->OnClickedButton.AddUObject(this, &UCGPauseUserWidget::OnClickedResumeButton);
        WidgetButtons.Add(ResumeButton);
    }

    if (OptionsButton)
    {
        OptionsButton->OnClickedButton.AddUObject(this, &UCGPauseUserWidget::OnClickedOptionsButton);
        WidgetButtons.Add(OptionsButton);
    }

    if (MenuButton)
    {
        MenuButton->OnClickedButton.AddUObject(this, &UCGPauseUserWidget::OnClickedMenuButton);
        WidgetButtons.Add(MenuButton);
    }

    if (QuitButton)
    {
        QuitButton->OnClickedButton.AddUObject(this, &UCGPauseUserWidget::OnClickedQuitButton);
        WidgetButtons.Add(QuitButton);
    }
}

void UCGPauseUserWidget::OnClickedResumeButton()
{
    GameStateToSet = EGameState::Game;
    ShowFadeoutAnimation();
}

void UCGPauseUserWidget::OnClickedOptionsButton()
{
    GameStateToSet = EGameState::Options;
    ShowFadeoutAnimation();
}

void UCGPauseUserWidget::OnClickedMenuButton()
{
    GameStateToSet = EGameState::MainMenu;
    ShowFadeoutAnimation();
}

void UCGPauseUserWidget::OnClickedQuitButton()
{
    const auto GameInstnce = GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    GameInstnce->QuitGame(GetOwningPlayer());
}

void UCGPauseUserWidget::SetGameState(EGameState NewGameState)
{
    const auto GameMode = GetGameModeBase();
    if (!GameMode)
        return;

    GameMode->SetGameState(NewGameState);

    if (NewGameState == EGameState::Game)
    {
        GameMode->ClearPause();
    }
}

void UCGPauseUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    if (GameStateToSet == EGameState::MainMenu)
    {
        const auto GameInstnce = GetGameInstance<UCGGameInstance>();
        if (!GameInstnce)
            return;

        GameInstnce->OpenMainMenu();
    }
    else
    {
        SetGameState(GameStateToSet);
    }
}
