// Cube_Game, All rights reserved.

#include "UI/Menu/CGPlayerRecordRowUserWidget.h"
#include "UI/CGTextUserWidget.h"

void UCGPlayerRecordRowUserWidget::SetNameText(const FText& NewText)
{
    NameText->SetText(NewText);
}

void UCGPlayerRecordRowUserWidget::SetScoreText(const FText& NewText)
{
    ScoreText->SetText(NewText);
}

void UCGPlayerRecordRowUserWidget::SetDateText(const FText& NewText)
{
    DateText->SetText(NewText);
}

void UCGPlayerRecordRowUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(NameText);
    check(ScoreText);
    check(DateText);
}
