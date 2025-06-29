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
    TransitionToGameState(EGameState::Game);
}

void UCGGameOverUserWidget::OnClickedMenuButton()
{
    TransitionToGameState(EGameState::MainMenu);
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
    if (NewGameState == EGameState::GameOver)
    {
        if (const auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
        {
            GameOverText->SetText(FormatGameOverText(GameMode->GetScore()));
        }
    }
}

void UCGGameOverUserWidget::OnNameTextChanged(const FText& NewText)
{
    UpdateAddButton();
}

FText UCGGameOverUserWidget::FormatGameOverText(int32 Score)
{
    if (GameOverFormat.ToString().Contains("{0}"))
    {
        return FText::FormatOrdered(GameOverFormat, LOCGEN_NUMBER_UNGROUPED(Score, ""));
    }

    return GameOverFormat;
}

void UCGGameOverUserWidget::OnFadeoutAnimationFinished()
{
    if (auto* GameInstnce = GetGameInstance<UCGGameInstance>())
    {
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
}
