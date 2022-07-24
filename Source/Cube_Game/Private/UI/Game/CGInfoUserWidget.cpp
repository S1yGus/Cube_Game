// Cube_Game, All rights reserved.

#include "UI/Game/CGInfoUserWidget.h"
#include "CGGameMode.h"
#include "Components/TextBlock.h"

void UCGInfoUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGInfoUserWidget::Setup()
{
    check(ScoreTextBlock);
    check(TimeTextBlock);
    check(SpeedTextBlock);

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
    ScoreTextBlock->SetText(FText::AsNumber(NewScore));
}

void UCGInfoUserWidget::OnTimeChanged(int32 NewTime)
{
    TimeTextBlock->SetText(FText::AsNumber(NewTime));
}

void UCGInfoUserWidget::OnSpeedChanged(int32 NewSpeed)
{
    SpeedTextBlock->SetText(FText::AsNumber(NewSpeed));
}
