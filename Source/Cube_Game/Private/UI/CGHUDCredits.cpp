// Cube_Game, All rights reserved.

#include "UI/CGHUDCredits.h"
#include "Blueprint/UserWidget.h"

void ACGHUDCredits::BeginPlay()
{
    Super::BeginPlay();

    check(AuthorsWidgetClass);
    check(TitleWidgetClass);
}

void ACGHUDCredits::SetupWidgets()
{
    GameWidgets.Add(EGameState::Authors, CreateWidget<UUserWidget>(GetWorld(), AuthorsWidgetClass));
    GameWidgets.Add(EGameState::Title, CreateWidget<UUserWidget>(GetWorld(), TitleWidgetClass));

    Super::SetupWidgets();
}
