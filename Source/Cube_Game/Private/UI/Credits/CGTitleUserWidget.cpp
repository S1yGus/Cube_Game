// Cube_Game, All rights reserved.

#include "UI/Credits/CGTitleUserWidget.h"
#include "Components/Button.h"
#include "CGGameInstance.h"

void UCGTitleUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(Button);

    Button->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
}

void UCGTitleUserWidget::OpenMainMenu()
{
    if (auto* GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>())
    {
        GameInstance->OpenMainMenu();
    }
}

void UCGTitleUserWidget::OnClicked()
{
    OpenMainMenu();
}

void UCGTitleUserWidget::OnStartupAnimationFinished()
{
    ShowFadeoutAnimation();
}

void UCGTitleUserWidget::OnFadeoutAnimationFinished()
{
    OpenMainMenu();
}
