// Cube_Game, All rights reserved.

#include "UI/Credits/CGAuthorsUserWidget.h"
#include "Components/Button.h"

void UCGAuthorsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(Button);

    Button->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
}

void UCGAuthorsUserWidget::OnClicked()
{
    SetGameState(EGameState::Title);
}

void UCGAuthorsUserWidget::OnStartupAnimationFinished()
{
    TransitionToGameState(EGameState::Title);
}
