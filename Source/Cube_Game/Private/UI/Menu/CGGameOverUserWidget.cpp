// Cube_Game, All rights reserved.

#include "UI/Menu/CGGameOverUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "CGGameInstance.h"
#include "CGGameMode.h"
#include "Components/EditableText.h"
#include "Kismet/KismetMathLibrary.h"

void UCGGameOverUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
    UpdateAddButton();
}

void UCGGameOverUserWidget::Setup()
{
    if (AddButton)
    {
        AddButton->OnClickedButton.AddUObject(this, &UCGGameOverUserWidget::OnClickedAddButton);
    }

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

    if (const auto GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &UCGGameOverUserWidget::OnGameStateChanged);
    }

    if (NameEditableText)
    {
        NameEditableText->OnTextChanged.AddDynamic(this, &UCGGameOverUserWidget::OnNameTextChanged);
    }
}

void UCGGameOverUserWidget::UpdateAddButton()
{
    if (!AddButton || !NameEditableText)
        return;

    const auto NameLen = NameEditableText->GetText().ToString().Len();
    AddButton->SetIsEnabled(!(NameLen < PlayerNameMinLen || NameLen > PlayerNameMaxLen));
}

void UCGGameOverUserWidget::OnClickedAddButton()
{
    PlayAnimation(AddAnimation);

    const auto GameInstnce = GetGameInstance<UCGGameInstance>();
    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameInstnce || !GameMode)
        return;

    FPlayerRecord PlayerRecord{NameEditableText->GetText(), GameMode->GetScore(), UKismetMathLibrary::Now()};
    GameInstnce->AddToLeaderboard(PlayerRecord);
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
    const auto GameInstnce = GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    GameInstnce->QuitGame(GetOwningPlayer());
}

void UCGGameOverUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::GameOver || !GameOverText)
        return;

    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode)
        return;

    GameOverText->SetText(FormatGameOverText(GameMode->GetScore()));
}

void UCGGameOverUserWidget::OnNameTextChanged(const FText& NewText)
{
    UpdateAddButton();
}

FText UCGGameOverUserWidget::FormatGameOverText(int32 Score)
{
    if (GameOverFormatStr.Contains("{0}"))
    {
        FStringFormatOrderedArguments GameOverArg;
        GameOverArg.Add(Score);
        return FText::FromString(FString::Format(*GameOverFormatStr, GameOverArg));
    }
    else
    {
        return FText::FromString(GameOverFormatStr);
    }
}

void UCGGameOverUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    const auto GameInstnce = GetGameInstance<UCGGameInstance>();
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
