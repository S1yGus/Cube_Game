// Cube_Game, All rights reserved.

#include "UI/Game/CGPopUpScoreUserWidget.h"
#include "CGGameMode.h"
#include "Components/TextBlock.h"
#include "Settings/CGGameUserSettings.h"

void UCGPopUpScoreUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ScoreTextBlock);

    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
    {
        GameMode->OnScoreChanged.AddUObject(this, &ThisClass::OnScoreChanged);
    }

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->OnPopUpTypeChanged.AddUObject(this, &ThisClass::OnPopUpTypeChanged);
        PopUpType = GameUserSettings->GetPopUpType();
    }
}

void UCGPopUpScoreUserWidget::OnScoreChanged(int32 NewScore, int32 DeltaScore, int32 NewMultiplier)
{
    if (PopUpType == EPopUpType::Off)
        return;

    ScoreTextBlock->SetText(FormatPopUpScore(DeltaScore, NewMultiplier));
    ScoreTextBlock->SetColorAndOpacity(FSlateColor{DeltaScore > 0 ? IncreaseScoreColor : DecreaseScoreColor});

    PlayAnimation(PopUpAnimation);
}

FText UCGPopUpScoreUserWidget::FormatPopUpScore(int32 DeltaScore, int32 NewMultiplier)
{
    const auto MultiplierStr = (NewMultiplier == 1) || (PopUpType == EPopUpType::Amount) ? FString{}    // If NewMultiplier == 1 no need to show the multiplier.
                                                                                         : FString::FromInt(NewMultiplier);
    const auto SpacerStr = (NewMultiplier == 1) || (PopUpType == EPopUpType::Amount) ? FString{}    // If NewMultiplier == 1 no need to show the multiplier.
                                                                                     : Spacer;
    const auto ScoreStr = FString::FromInt(FMath::Abs(PopUpType == EPopUpType::Amount ? DeltaScore : DeltaScore / NewMultiplier));
    return FText::FromString(FString{MultiplierStr + SpacerStr + ScoreStr});
}
