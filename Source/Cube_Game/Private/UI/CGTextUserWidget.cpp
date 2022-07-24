// Cube_Game, All rights reserved.

#include "UI/CGTextUserWidget.h"
#include "Components/TextBlock.h"

void UCGTextUserWidget::SetText(const FText& NewText)
{
    Text = NewText;
    TextBlock->SetText(NewText);
}

void UCGTextUserWidget::SetColor(const FLinearColor& NewColor)
{
    TextBlock->SetColorAndOpacity(FSlateColor{NewColor});
}

void UCGTextUserWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    check(TextBlock);

    TextBlock->SetText(Text);
}
