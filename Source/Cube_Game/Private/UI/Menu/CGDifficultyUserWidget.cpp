// Cube_Game, All rights reserved.

#include "UI/Menu/CGDifficultyUserWidget.h"
#include "UI/Menu/CGButtonUserWidget.h"
#include "Components/VerticalBox.h"
#include "CGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"
#include "Settings/CGGameUserSettings.h"
#include "CGUtils.h"

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
    if (const FText DifficultyDisplayName = CubeGame::Utils::GetDifficultyDisplayName(Difficulty);    //
        !DifficultyDisplayName.IsEmpty())
    {
        auto* Button = CreateWidget<UCGButtonUserWidget>(GetWorld(), DifficultyButtonWidgetClass);
        check(Button);
        Button->SetText(DifficultyDisplayName);
        Button->OnClickedButton.AddLambda(
            [Difficulty, this]()
            {
                if (auto* GameUserSettings = UCGGameUserSettings::Get())
                {
                    GameUserSettings->SetDifficulty(Difficulty);
                    ShowFadeoutAnimation();
                    UGameplayStatics::PushSoundMixModifier(this, FadeOutSoundMix);    // Smoothly mutes all sounds.
                }
            });
        DifficultyButtonsVerticalBox->AddChild(Button);
    }
}

void UCGDifficultyUserWidget::OnFadeoutAnimationFinished()
{
    if (auto* GameInstnce = GetGameInstance<UCGGameInstance>())
    {
        GameInstnce->StartGame();
        UGameplayStatics::PopSoundMixModifier(this, FadeOutSoundMix);    // Smoothly returns all sounds.
    }
}
