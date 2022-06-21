// Cube_Game, All rights reserved.

#include "UI/Menu/CGDifficultyUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameInstance.h"

void UCGDifficultyUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (EasyButton)
    {
        EasyButton->OnClickedButton.AddUObject(this, &UCGDifficultyUserWidget::OnClickedEasyButton);
    }

    if (MediumButton)
    {
        MediumButton->OnClickedButton.AddUObject(this, &UCGDifficultyUserWidget::OnClickedMediumButton);
    }

    if (HardButton)
    {
        HardButton->OnClickedButton.AddUObject(this, &UCGDifficultyUserWidget::OnClickedHardButton);
    }
}

void UCGDifficultyUserWidget::OnClickedEasyButton()
{
    SetDifficulty(EDifficulty::Easy);
    ShowFadeoutAnimation();
}

void UCGDifficultyUserWidget::OnClickedMediumButton()
{
    SetDifficulty(EDifficulty::Medium);
    ShowFadeoutAnimation();
}

void UCGDifficultyUserWidget::OnClickedHardButton()
{
    SetDifficulty(EDifficulty::Hard);
    ShowFadeoutAnimation();
}

void UCGDifficultyUserWidget::SetDifficulty(EDifficulty NewDifficulty)
{
    const auto GameInstnce = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    GameInstnce->SetDifficulty(NewDifficulty);
}

void UCGDifficultyUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    const auto GameInstnce = GetWorld()->GetGameInstance<UCGGameInstance>();
    if (!GameInstnce)
        return;

    GameInstnce->StartGame();
}
