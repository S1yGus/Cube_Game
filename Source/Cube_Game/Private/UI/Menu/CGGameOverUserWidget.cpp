// Cube_Game, All rights reserved.

#include "UI/Menu/CGGameOverUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "CGGameInstance.h"
#include "CGGameMode.h"
#include "Components/EditableText.h"

void UCGGameOverUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
    UpdateAddButton();
}

void UCGGameOverUserWidget::Setup()
{
    check(AddButton);
    check(PlayAgainButton);
    check(MenuButton);
    check(QuitButton);
    check(NameEditableText);

    AddButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedAddButton);
    PlayAgainButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedPlayAgainButton);
    MenuButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedMenuButton);
    QuitButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedQuitButton);
    NameEditableText->OnTextChanged.AddDynamic(this, &ThisClass::OnNameTextChanged);

    if (ACGGameModeBase* GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }
}

void UCGGameOverUserWidget::UpdateAddButton()
{
    const auto NameLen = NameEditableText->GetText().ToString().Len();
    AddButton->SetIsEnabled(!(NameLen < PlayerNameMinLen || NameLen > PlayerNameMaxLen));
}

void UCGGameOverUserWidget::OnClickedAddButton()
{
    PlayAnimation(AddAnimation);

    auto* GameInstnce = GetGameInstance<UCGGameInstance>();
    const auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr;
    if (!GameInstnce || !GameMode)
        return;

    const FPlayerRecord PlayerRecord{NameEditableText->GetText(), GameMode->GetScore(), FDateTime::Now()};
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
    if (auto* GameInstnce = GetGameInstance<UCGGameInstance>())
    {
        GameInstnce->QuitGame(GetOwningPlayer());
    }
}

void UCGGameOverUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::GameOver)
        return;

    const auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr;
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
    if (GameOverFormat.ToString().Contains("{0}"))
    {
        FStringFormatOrderedArguments GameOverArg;
        GameOverArg.Add(Score);
        return FText::FromString(FString::Format(*GameOverFormat.ToString(), GameOverArg));
    }

    return GameOverFormat;
}

void UCGGameOverUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    auto* GameInstnce = GetGameInstance<UCGGameInstance>();
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
