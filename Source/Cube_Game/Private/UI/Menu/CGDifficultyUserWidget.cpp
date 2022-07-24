// Cube_Game, All rights reserved.

#include "UI/Menu/CGDifficultyUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameInstance.h"

void UCGDifficultyUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(EasyButton);
    check(MediumButton);
    check(HardButton);

    EasyButton->OnClickedButton.AddUObject(this, &UCGDifficultyUserWidget::OnClickedEasyButton);
    MediumButton->OnClickedButton.AddUObject(this, &UCGDifficultyUserWidget::OnClickedMediumButton);
    HardButton->OnClickedButton.AddUObject(this, &UCGDifficultyUserWidget::OnClickedHardButton);
}

void UCGDifficultyUserWidget::SetDifficulty(EDifficulty NewDifficulty)
{
    const auto GameInstnce = GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    GameInstnce->SetDifficulty(NewDifficulty);
}

void UCGDifficultyUserWidget::OnClickedEasyButton()
{
    if (IsAnyAnimationPlaying())
        return;

    SetDifficulty(EDifficulty::Easy);
    ShowFadeoutAnimation();
}

void UCGDifficultyUserWidget::OnClickedMediumButton()
{
    if (IsAnyAnimationPlaying())
        return;

    SetDifficulty(EDifficulty::Medium);
    ShowFadeoutAnimation();
}

void UCGDifficultyUserWidget::OnClickedHardButton()
{
    if (IsAnyAnimationPlaying())
        return;

    SetDifficulty(EDifficulty::Hard);
    ShowFadeoutAnimation();
}

void UCGDifficultyUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    const auto GameInstnce = GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    GameInstnce->StartGame();
}
