// Cube_Game, All rights reserved.

#include "UI/Menu/CGPopUpHintUserWidget.h"
#include "CGGameMode.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Components/TextBlock.h"

void UCGPopUpHintUserWidget::ResetWidget()
{
    ResumeButton->ResetButton();
}

void UCGPopUpHintUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
    {
        GameMode->OnShowPopUpHintSignature.AddUObject(this, &UCGPopUpHintUserWidget::OnShowPopUpHint);
    }

    if (ResumeButton)
    {
        ResumeButton->OnClickedButton.AddUObject(this, &UCGPopUpHintUserWidget::OnClickedResumeButton);
    }
}

void UCGPopUpHintUserWidget::OnShowPopUpHint(const FPopUpHint& PopUpHint)
{
    if (!HintTextBlock || !TitleTextBlock)
        return;

    TitleTextBlock->SetText(PopUpHint.Title);
    TitleTextBlock->SetColorAndOpacity(PopUpHint.TitleColor);
    HintTextBlock->SetText(PopUpHint.HintText);
}

void UCGPopUpHintUserWidget::OnClickedResumeButton()
{
    ShowFadeoutAnimation();
}

void UCGPopUpHintUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode)
        return;

    GameMode->ClearPause();
}
