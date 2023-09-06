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

    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
    {
        GameMode->OnScoreChanged.AddUObject(this, &ThisClass::OnScoreChanged);
        GameMode->OnTimeChanged.AddUObject(this, &ThisClass::OnTimeChanged);
        GameMode->OnSpeedChanged.AddUObject(this, &ThisClass::OnSpeedChanged);

        OnScoreChanged(GameMode->GetScore());
        OnTimeChanged(GameMode->GetGameTime());
        OnSpeedChanged(GameMode->GetGameSpeed());
    }
}

void UCGInfoUserWidget::OnScoreChanged(int32 NewScore, int32 DeltaScore, int32 NewMultiplier)
{
    ScoreTextBlock->SetText(FText::FromString(FString::FromInt(NewScore)));
}

void UCGInfoUserWidget::OnTimeChanged(int32 NewTime)
{
    TimeTextBlock->SetText(FText::AsNumber(NewTime));
}

void UCGInfoUserWidget::OnSpeedChanged(int32 NewSpeed)
{
    SpeedTextBlock->SetText(FText::AsNumber(NewSpeed));
}
