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
    check(HowButton);
    check(OptionsButton);
    check(MenuButton);
    check(QuitButton);

    ResumeButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedResumeButton);
    WidgetButtons.Add(ResumeButton);
    HowButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedHowButton);
    WidgetButtons.Add(HowButton);
    OptionsButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedOptionsButton);
    WidgetButtons.Add(OptionsButton);
    MenuButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedMenuButton);
    WidgetButtons.Add(MenuButton);
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

void UCGPauseUserWidget::ResetWidget()
{
    for (auto& Button : WidgetButtons)
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

void UCGPauseUserWidget::OnPressedEscape()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedResumeButton();
}

void UCGPauseUserWidget::OnClickedResumeButton()
{
    ShowFadeoutAnimationAndSetGameState(EGameState::Game);
}

void UCGPauseUserWidget::OnClickedHowButton()
{
    ShowFadeoutAnimationAndSetGameState(EGameState::HowToPlay);
}

void UCGPauseUserWidget::OnClickedOptionsButton()
{
    ShowFadeoutAnimationAndSetGameState(EGameState::Options);
}

void UCGPauseUserWidget::OnClickedMenuButton()
{
    ShowFadeoutAnimationAndSetGameState(EGameState::MainMenu);
}

void UCGPauseUserWidget::OnClickedQuitButton()
{
    if (auto* GameInstnce = GetGameInstance<UCGGameInstance>())
    {
        GameInstnce->QuitGame(GetOwningPlayer());
    }
}

void UCGPauseUserWidget::SetGameState(EGameState NewGameState)
{
    Super::SetGameState(NewGameState);

    if (NewGameState == EGameState::Game && GetGameModeBase())
    {
        GetGameModeBase()->ClearPause();
    }
}

void UCGPauseUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == FadeoutAnimation)
    {
        if (GameStateToSet == EGameState::MainMenu)
        {
            if (auto* GameInstnce = GetGameInstance<UCGGameInstance>())
            {
                GameInstnce->OpenMainMenu();
            }
        }
        else
        {
            SetGameState(GameStateToSet);
        }
    }
}
