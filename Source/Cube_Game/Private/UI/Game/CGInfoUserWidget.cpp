// Cube_Game, All rights reserved.

#include "UI/Game/CGInfoUserWidget.h"
#include "CGGameMode.h"
#include "Components/TextBlock.h"
#include "CGUtils.h"

void UCGInfoUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
    {
        GameMode->OnScoreChanged.AddUObject(this, &UCGInfoUserWidget::OnScoreChanged);
        GameMode->OnTimeChanged.AddUObject(this, &UCGInfoUserWidget::OnTimeChanged);
        GameMode->OnSpeedChanged.AddUObject(this, &UCGInfoUserWidget::OnSpeedChanged);
        OnScoreChanged(GameMode->GetScore());
        OnTimeChanged(GameMode->GetTime());
        OnSpeedChanged(GameMode->GetSpeed());
    }
}

void UCGInfoUserWidget::OnScoreChanged(int32 NewScore, int32 DeltaScore, int32 NewMultiplier)
{
    if (!ScoreTextBlock)
        return;

    ScoreTextBlock->SetText(CGUtils::TextFromInt(NewScore));
}

void UCGInfoUserWidget::OnTimeChanged(int32 NewTime)
{
    if (!TimeTextBlock)
        return;

    TimeTextBlock->SetText(CGUtils::TextFromInt(NewTime));
}

void UCGInfoUserWidget::OnSpeedChanged(int32 NewSpeed)
{
    if (!SpeedTextBlock)
        return;

    SpeedTextBlock->SetText(CGUtils::TextFromInt(NewSpeed));
}
