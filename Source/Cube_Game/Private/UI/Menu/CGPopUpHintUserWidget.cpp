// Cube_Game, All rights reserved.

#include "UI/Menu/CGPopUpHintUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "CGGameMode.h"
#include "Components/TextBlock.h"
#include "Player/CGPlayerController.h"

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

    ResumeButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedResumeButton);

    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
    {
        GameMode->OnShowPopUpHint.AddUObject(this, &ThisClass::OnShowPopUpHint);
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (auto* PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEnt.AddUObject(this, &ThisClass::OnPressedEnter);
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEnter);
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
    if (!IsVisible() || IsAnyAnimationPlaying())
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
    ShowFadeoutAnimation();
}

void UCGPopUpHintUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    ACGGameModeBase* GameMode = GetGameModeBase();
    if (!GameMode)
        return;

    GameMode->ClearPause();
}
