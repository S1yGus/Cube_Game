// Cube_Game, All rights reserved.

#include "UI/Menu/CGMainMenuUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameInstance.h"
#include "CGGameModeBase.h"

void UCGMainMenuUserWidget::ResetWidget()
{
    for (const auto& Button : WidgetButtons)
    {
        Button->ResetButton();
    }
}

void UCGMainMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GameButton)
    {
        GameButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedGameButton);
        WidgetButtons.Add(GameButton);
    }

    if (OptionsButton)
    {
        OptionsButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedOptionsButton);
        WidgetButtons.Add(OptionsButton);
    }

    if (QuitButton)
    {
        QuitButton->OnClickedButton.AddUObject(this, &UCGMainMenuUserWidget::OnClickedQuitButton);
        WidgetButtons.Add(QuitButton);
    }
}

void UCGMainMenuUserWidget::OnClickedGameButton()
{
    GameStateToSet = EGameState::DifficultySelection;
    ShowFadeoutAnimation();
}

void UCGMainMenuUserWidget::OnClickedOptionsButton()
{
    GameStateToSet = EGameState::Options;
    ShowFadeoutAnimation();
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
