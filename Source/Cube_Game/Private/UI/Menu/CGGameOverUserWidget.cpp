// Cube_Game, All rights reserved.

#include "UI/Menu/CGGameOverUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameInstance.h"
#include "CGGameMode.h"
#include "Components/TextBlock.h"

void UCGGameOverUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (PlayAgainButton)
    {
        PlayAgainButton->OnClickedButton.AddUObject(this, &UCGGameOverUserWidget::OnClickedPlayAgainButton);
    }

    if (MenuButton)
    {
        MenuButton->OnClickedButton.AddUObject(this, &UCGGameOverUserWidget::OnClickedMenuButton);
    }

    if (QuitButton)
    {
        QuitButton->OnClickedButton.AddUObject(this, &UCGGameOverUserWidget::OnClickedQuitButton);
    }

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameModeBase>())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &UCGGameOverUserWidget::OnGameStateChanged);
    }
}

void UCGGameOverUserWidget::OnClickedPlayAgainButton()
{
    GameStateToSet = EGameState::Game;
    ShowFadeoutAnimation();
}

void UCGGameOverUserWidget::OnClickedMenuButton()
{
    GameStateToSet = EGameState::MainMenu;
    ShowFadeoutAnimation();
}

void UCGGameOverUserWidget::OnClickedQuitButton()
{
    const auto GameInstnce = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    GameInstnce->QuitGame(GetOwningPlayer());
}

void UCGGameOverUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::GameOver || !ScoreTextBlock)
        return;

    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode)
        return;

    ScoreTextBlock->SetText(FormatGameOverText(GameMode->GetScore()));
}

FText UCGGameOverUserWidget::FormatGameOverText(int32 Score)
{
    return FText::FromString(FString::Printf(TEXT("You have scored %d points."), Score));
}

void UCGGameOverUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    const auto GameInstnce = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    switch (GameStateToSet)
    {
        case EGameState::Game:
            GameInstnce->StartGame();
            break;
        case EGameState::MainMenu:
            GameInstnce->OpenMainMenu();
            break;
        default:
            break;
    }
}
