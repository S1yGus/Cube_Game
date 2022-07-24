// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGCoreTypes.h"
#include "CGButtonUserWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class CUBE_GAME_API UCGButtonUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnClickedButtonSignature OnClickedButton;

    void ResetButton();
    void SetText(const FText& NewText);

protected:
    UPROPERTY(Meta = (BindWidget))
    UButton* Button;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ButtonTextBlock;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* OnClickedAnimation;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* OnHoveredAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FText ButtonText;

    virtual void NativeOnInitialized() override;
    virtual void NativePreConstruct() override;

private:
    void Setup();

    UFUNCTION()
    void OnClicked();
    UFUNCTION()
    void OnHovered();
    UFUNCTION()
    void OnUnhovered();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
