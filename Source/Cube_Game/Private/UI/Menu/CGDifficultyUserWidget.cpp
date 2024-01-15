// Cube_Game, All rights reserved.

#include "UI/Menu/CGDifficultyUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Components/VerticalBox.h"
#include "CGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"
#include "Settings/CGGameUserSettings.h"

void UCGDifficultyUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(DifficultyButtonsVerticalBox);

    InitDifficultyButtons();
}

void UCGDifficultyUserWidget::InitDifficultyButtons()
{
    DifficultyButtonsVerticalBox->ClearChildren();

    const UEnum* DifficultyEnum = StaticEnum<EDifficulty>();
    for (int32 i = 0; i < DifficultyEnum->NumEnums() - 1; ++i)
    {
        CreateAndAddDifficultyButton(static_cast<EDifficulty>(i));
    }
}

void UCGDifficultyUserWidget::CreateAndAddDifficultyButton(EDifficulty Difficulty)
{
    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        if (const FText DifficultyDisplayName = GameUserSettings->GetDifficultyDisplayName(Difficulty);    //
            !DifficultyDisplayName.IsEmpty())
        {
            auto* Button = CreateWidget<UCGButtonUserWidget>(GetWorld(), DifficultyButtonWidgetClass);
            check(Button);
            Button->SetText(DifficultyDisplayName);
            Button->OnClickedButton.AddLambda(
                [=, this]()
                {
                    GameUserSettings->SetDifficulty(Difficulty);
                    ShowFadeoutAnimation();
                    UGameplayStatics::PushSoundMixModifier(this, FadeOutSoundMix);    // Smoothly mutes all sounds.
                });
            DifficultyButtonsVerticalBox->AddChild(Button);
        }
    }
}

void UCGDifficultyUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == FadeoutAnimation)
    {
        if (auto* GameInstnce = GetGameInstance<UCGGameInstance>())
        {
            GameInstnce->StartGame();
            UGameplayStatics::PopSoundMixModifier(this, FadeOutSoundMix);    // Smoothly returns all sounds.
        }
    }
}
