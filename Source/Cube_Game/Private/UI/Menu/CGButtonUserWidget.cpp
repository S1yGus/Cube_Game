// Cube_Game, All rights reserved.

#include "UI/Menu/CGButtonUserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCGButtonUserWidget::ResetButton()
{
    // Returning the button to a normal state after animation.
    ButtonTextBlock->SetRenderScale(FVector2D{1.0});
    ButtonTextBlock->SetRenderOpacity(1.0f);
    Button->SetIsEnabled(true);
}

void UCGButtonUserWidget::SetText(const FText& NewText)
{
    ButtonText = NewText;
    ButtonTextBlock->SetText(ButtonText);
}

void UCGButtonUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Setup();
}

void UCGButtonUserWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    ButtonTextBlock->SetText(ButtonText);
}

void UCGButtonUserWidget::Setup()
{
    check(Button);
    check(ButtonTextBlock);

    Button->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
    Button->OnHovered.AddDynamic(this, &ThisClass::OnHovered);
    Button->OnUnhovered.AddDynamic(this, &ThisClass::OnUnhovered);
}

void UCGButtonUserWidget::OnClicked()
{
    Button->SetIsEnabled(false);
    PlayAnimation(OnClickedAnimation);
}

void UCGButtonUserWidget::OnHovered()
{
    PlayAnimationForward(OnHoveredAnimation);
}

void UCGButtonUserWidget::OnUnhovered()
{
    PlayAnimationReverse(OnHoveredAnimation);
}

void UCGButtonUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == OnClickedAnimation)
    {
        OnClickedButton.Broadcast();
        Button->SetIsEnabled(true);
    }
}
