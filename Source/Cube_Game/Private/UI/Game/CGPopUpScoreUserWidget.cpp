// Cube_Game, All rights reserved.

#include "UI/Game/CGPopUpScoreUserWidget.h"
#include "CGGameMode.h"
#include "Components/TextBlock.h"
#include "CGGameInstance.h"
#include "Saves/CGSettingsSave.h"

void UCGPopUpScoreUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
    {
        GameMode->OnScoreChanged.AddUObject(this, &UCGPopUpScoreUserWidget::OnScoreChanged);
    }
}

void UCGPopUpScoreUserWidget::OnScoreChanged(int32 NewScore, int32 DeltaScore, int32 NewMultiplier)
{
    if (!ScoreTextBlock || !PopUpAnimation)
        return;

    const auto GameInstance = GetGameInstance<UCGGameInstance>();
    const auto SettingsSave = GameInstance->GetSettingsSave();
    const auto PopUpType = SettingsSave->GetGameSettings().PopUp;
    if (PopUpType == EPopUpType::Off)
        return;

    ScoreTextBlock->SetText(FormatPopUpScore(DeltaScore, NewMultiplier, PopUpType));
    ScoreTextBlock->SetColorAndOpacity(FSlateColor{DeltaScore > 0 ? IncreaseScoreColor : DecreaseScoreColor});

    PlayAnimation(PopUpAnimation);
}

FText UCGPopUpScoreUserWidget::FormatPopUpScore(int32 DeltaScore, int32 NewMultiplier, EPopUpType PopUpType)
{
    const auto MultiplierStr = (NewMultiplier == 1) || (PopUpType == EPopUpType::Amount) ? FString{} : FString::FromInt(NewMultiplier);
    const auto SpacerStr = (NewMultiplier == 1) || (PopUpType == EPopUpType::Amount) ? FString{} : Spacer;
    const auto ScoreStr = FString::FromInt(FMath::Abs(PopUpType == EPopUpType::Amount ? DeltaScore : DeltaScore / NewMultiplier));
    return FText::FromString(FString{MultiplierStr + SpacerStr + ScoreStr});
}
