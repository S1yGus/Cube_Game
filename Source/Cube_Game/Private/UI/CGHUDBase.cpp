// Cube_Game, All rights reserved.

#include "UI/CGHUDBase.h"
#include "CGGameModeBase.h"
#include "UI/CGAnimatedUserWidget.h"

void ACGHUDBase::BeginPlay()
{
    Super::BeginPlay();

    check(OptionsWidgetClass);
    check(OptionsWarningWidgetClass);
    check(HowToPlayWidgetClass);

    SetupWidgets();

    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameModeBase>() : nullptr)
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }
}

void ACGHUDBase::SetupWidgets()
{
    GameWidgets.Add(EGameState::Options, CreateWidget<UUserWidget>(GetWorld(), OptionsWidgetClass));
    GameWidgets.Add(EGameState::OptionsWarning, CreateWidget<UUserWidget>(GetWorld(), OptionsWarningWidgetClass));
    GameWidgets.Add(EGameState::HowToPlay, CreateWidget<UUserWidget>(GetWorld(), HowToPlayWidgetClass));

    for (const auto& [GameState, GameWidget] : GameWidgets)
    {
        if (GameWidget)
        {
            GameWidget->AddToViewport();
            GameWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void ACGHUDBase::OnGameStateChanged(EGameState GameState)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (GameWidgets.Contains(GameState))
    {
        CurrentWidget = GameWidgets[GameState];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);

        if (auto* AnimatedWidget = Cast<UCGAnimatedUserWidget>(CurrentWidget))
        {
            AnimatedWidget->ShowStartupAnimation();
        }
    }
}
