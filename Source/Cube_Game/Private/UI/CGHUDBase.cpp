// Cube_Game, All rights reserved.

#include "UI/CGHUDBase.h"
#include "CGGameModeBase.h"
#include "UI/CGAnimatedUserWidget.h"

void ACGHUDBase::BeginPlay()
{
    Super::BeginPlay();

    SetupWidgets();
}

void ACGHUDBase::SetupWidgets()
{
    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameModeBase>())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ACGHUDBase::OnGameStateChanged);
    }

    if (OptionsWidgetClass)
    {
        GameWidgets.Add(EGameState::Options, CreateWidget<UUserWidget>(GetWorld(), OptionsWidgetClass));
    }

    if (OptionsWarningWidgetClass)
    {
        GameWidgets.Add(EGameState::OptionsWarning, CreateWidget<UUserWidget>(GetWorld(), OptionsWarningWidgetClass));
    }

    for (const auto& GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget)
            continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Collapsed);
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

        if (const auto AnimatedWidget = Cast<UCGAnimatedUserWidget>(CurrentWidget))
        {
            AnimatedWidget->ShowStartupAnimation();
        }
    }
}
