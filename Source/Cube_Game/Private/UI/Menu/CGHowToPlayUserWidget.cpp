// Cube_Game, All rights reserved.

#include "UI/Menu/CGHowToPlayUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "UI/CGTextUserWidget.h"
#include "CGGameModeBase.h"
#include "Components/TextBlock.h"
#include "UI/CGHUDBase.h"
#include "CGGameInstance.h"
#include "Player/CGPlayerController.h"

void UCGHowToPlayUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
    ShowHint(CurrentHintIndex);
}

void UCGHowToPlayUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == FadeoutHintAnimation)
    {
        ShowHint(CurrentHintIndex);
        PlayAnimation(FadeInHintAnimation);
    }
}

void UCGHowToPlayUserWidget::Setup()
{
    check(TitleText);
    check(HintTextBlock);
    check(BackButton);
    check(NextButton);
    check(PrevButton);

    BackButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedBackButton);
    NextButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedNextButton);
    PrevButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedPrevButton);

    if (ACGGameModeBase* GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (auto* PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEscape.AddUObject(this, &ThisClass::OnPressedEscape);
    }
}

void UCGHowToPlayUserWidget::ResetWidget()
{
    BackButton->ResetButton();
}

void UCGHowToPlayUserWidget::ShowHint(int32 HintIndex)
{
    if (const auto* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<UCGGameInstance>() : nullptr)
    {
        const auto Hints = GameInstance->GetHints();
        TitleText->SetText(Hints[HintIndex].Title);
        TitleText->SetColor(Hints[HintIndex].TitleColor);
        HintTextBlock->SetText(Hints[HintIndex].HintText);
    }
}

void UCGHowToPlayUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState == EGameState::HowToPlay)
    {
        ResetWidget();
    }
}

void UCGHowToPlayUserWidget::OnClickedBackButton()
{
    ShowFadeoutAnimation();
}

void UCGHowToPlayUserWidget::OnClickedNextButton()
{
    if (IsAnyAnimationPlaying())
        return;

    if (const auto* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<UCGGameInstance>() : nullptr)
    {
        CurrentHintIndex = (CurrentHintIndex + 1) % GameInstance->GetHints().Num();
    }

    PlayAnimation(FadeoutHintAnimation);
}

void UCGHowToPlayUserWidget::OnClickedPrevButton()
{
    if (IsAnyAnimationPlaying())
        return;

    if (const auto* GameInstance = GetWorld() ? GetWorld()->GetGameInstance<UCGGameInstance>() : nullptr)
    {
        CurrentHintIndex = CurrentHintIndex - 1 < 0 ? GameInstance->GetHints().Num() - 1 : CurrentHintIndex - 1;
        PlayAnimation(FadeoutHintAnimation);
    }
}

void UCGHowToPlayUserWidget::OnPressedEscape()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedBackButton();
}

void UCGHowToPlayUserWidget::OnFadeoutAnimationFinished()
{
    if (auto* HUD = GetOwningPlayer() ? GetOwningPlayer()->GetHUD<ACGHUDBase>() : nullptr)
    {
        HUD->BackToRootMenu();
    }
}
