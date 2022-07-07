// Cube_Game, All rights reserved.

#include "UI/Menu/CGPlayerRecordRowUserWidget.h"
#include "UI/CGTextUserWidget.h"

void UCGPlayerRecordRowUserWidget::SetNameText(const FText& NewText)
{
    if (!NameText)
        return;

    NameText->SetText(NewText);
}

void UCGPlayerRecordRowUserWidget::SetScoreText(const FText& NewText)
{
    if (!ScoreText)
        return;

    ScoreText->SetText(NewText);
}

void UCGPlayerRecordRowUserWidget::SetDateText(const FText& NewText)
{
    if (!DateText)
        return;

    DateText->SetText(NewText);
}
