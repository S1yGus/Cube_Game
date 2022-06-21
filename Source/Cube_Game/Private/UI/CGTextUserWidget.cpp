// Cube_Game, All rights reserved.

#include "UI/CGTextUserWidget.h"
#include "Components/TextBlock.h"

void UCGTextUserWidget::SetText(const FText& NewText)
{
    if (!TextBlock)
        return;

    TextBlock->SetText(NewText);
}

void UCGTextUserWidget::SetColor(const FLinearColor& NewColor)
{
    if (!TextBlock)
        return;

    TextBlock->SetColorAndOpacity(FSlateColor{NewColor});
}

void UCGTextUserWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (TextBlock)
    {
        TextBlock->SetText(Text);
    }
}
