// Cube_Game, All rights reserved.

#include "UI/Menu/CGPopUpHintUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "CGGameMode.h"
#include "Components/TextBlock.h"

void UCGPopUpHintUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGPopUpHintUserWidget::Setup()
{
    check(ResumeButton);
    check(TitleText);
    check(HintTextBlock);

    ResumeButton->OnClickedButton.AddUObject(this, &UCGPopUpHintUserWidget::OnClickedResumeButton);

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
    {
        GameMode->OnShowPopUpHintSignature.AddUObject(this, &UCGPopUpHintUserWidget::OnShowPopUpHint);
        GameMode->OnGameStateChanged.AddUObject(this, &UCGPopUpHintUserWidget::OnGameStateChanged);
        GameMode->OnPressedEnt.AddUObject(this, &UCGPopUpHintUserWidget::OnPressedEnter);
        GameMode->OnPressedEsc.AddUObject(this, &UCGPopUpHintUserWidget::OnPressedEnter);
    }
}

void UCGPopUpHintUserWidget::ResetWidget()
{
    ResumeButton->ResetButton();
}

void UCGPopUpHintUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::PopUpHint)
        return;

    ResetWidget();
}

void UCGPopUpHintUserWidget::OnPressedEnter()
{
    if (!IsVisible())
        return;

    OnClickedResumeButton();
}

void UCGPopUpHintUserWidget::OnShowPopUpHint(const FHintData& HintData)
{
    TitleText->SetText(HintData.Title);
    TitleText->SetColor(HintData.TitleColor);
    HintTextBlock->SetText(HintData.HintText);
}

void UCGPopUpHintUserWidget::OnClickedResumeButton()
{
    if (IsAnyAnimationPlaying())
        return;

    ShowFadeoutAnimation();
}

void UCGPopUpHintUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    const auto GameMode = GetGameModeBase();
    if (!GameMode)
        return;

    GameMode->ClearPause();
}
