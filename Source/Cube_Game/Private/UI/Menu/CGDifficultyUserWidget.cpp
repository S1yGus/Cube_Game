// Cube_Game, All rights reserved.

#include "UI/Menu/CGDifficultyUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "CGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"

void UCGDifficultyUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(EasyButton);
    check(MediumButton);
    check(HardButton);

    EasyButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedEasyButton);
    MediumButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedMediumButton);
    HardButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedHardButton);
}

void UCGDifficultyUserWidget::SetDifficulty(EDifficulty NewDifficulty)
{
    if (const auto GameInstnce = GetGameInstance<UCGGameInstance>())
    {
        GameInstnce->SetDifficulty(NewDifficulty);
        ShowFadeoutAnimation();
        UGameplayStatics::PushSoundMixModifier(this, FadeOutSoundMix);    // Smoothly mutes all sounds.
    }
}

void UCGDifficultyUserWidget::OnClickedEasyButton()
{
    SetDifficulty(EDifficulty::Easy);
}

void UCGDifficultyUserWidget::OnClickedMediumButton()
{
    SetDifficulty(EDifficulty::Medium);
}

void UCGDifficultyUserWidget::OnClickedHardButton()
{
    SetDifficulty(EDifficulty::Hard);
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
    UGameplayStatics::PopSoundMixModifier(this, FadeOutSoundMix);    // Smoothly returns all sounds.
}
