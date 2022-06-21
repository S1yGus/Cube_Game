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

void UCGButtonUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UCGButtonUserWidget::OnClicked);
        Button->OnHovered.AddDynamic(this, &UCGButtonUserWidget::OnHovered);
        Button->OnUnhovered.AddDynamic(this, &UCGButtonUserWidget::OnUnhovered);
    }
}

void UCGButtonUserWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ButtonTextBlock)
    {
        ButtonTextBlock->SetText(ButtonText);
    }
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
    }
}
