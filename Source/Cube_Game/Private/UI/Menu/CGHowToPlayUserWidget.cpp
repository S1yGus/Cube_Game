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

    if (const auto GameMode = GetGameModeBase())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (const auto PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEsc);
    }
}

void UCGHowToPlayUserWidget::ResetWidget()
{
    BackButton->ResetButton();
}

void UCGHowToPlayUserWidget::ShowHint(int32 HintIndex)
{
    if (const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>())
    {
        TitleText->SetText(GameInstance->GetHints()[HintIndex].Title);
        TitleText->SetColor(GameInstance->GetHints()[HintIndex].TitleColor);
        HintTextBlock->SetText(GameInstance->GetHints()[HintIndex].HintText);
    }
}

void UCGHowToPlayUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::HowToPlay)
        return;

    ResetWidget();
}

void UCGHowToPlayUserWidget::OnClickedBackButton()
{
    ShowFadeoutAnimation();
}

void UCGHowToPlayUserWidget::OnClickedNextButton()
{
    if (IsAnyAnimationPlaying())
        return;

    if (const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>())
    {
        CurrentHintIndex = (CurrentHintIndex + 1) % GameInstance->GetHints().Num();
    }

    PlayAnimation(FadeoutHintAnimation);
}

void UCGHowToPlayUserWidget::OnClickedPrevButton()
{
    if (IsAnyAnimationPlaying())
        return;

    if (const auto GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>())
    {
        CurrentHintIndex = CurrentHintIndex - 1 < 0 ? GameInstance->GetHints().Num() - 1 : CurrentHintIndex - 1;
    }

    PlayAnimation(FadeoutHintAnimation);
}

void UCGHowToPlayUserWidget::OnPressedEsc()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedBackButton();
}

void UCGHowToPlayUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == FadeoutAnimation)
    {
        const auto OwnerController = GetOwningPlayer();
        if (!OwnerController)
            return;

        const auto HUD = OwnerController->GetHUD<ACGHUDBase>();
        if (!HUD)
            return;

        HUD->BackToRootMenu();
    }

    if (Animation == FadeoutHintAnimation)
    {
        ShowHint(CurrentHintIndex);
        PlayAnimation(FadeInHintAnimation);
    }
}
