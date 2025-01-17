// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGCoreTypes.h"
#include "CGPopUpScoreUserWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class CUBE_GAME_API UCGPopUpScoreUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> ScoreTextBlock;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> PopUpAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Colors")
    FLinearColor IncreaseScoreColor{FLinearColor::Green};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Colors")
    FLinearColor DecreaseScoreColor{FLinearColor::Red};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Colors")
    FString Spacer{"x"};

    virtual void NativeOnInitialized() override;

private:
    EPopUpType PopUpType{EPopUpType::Multiplier};

    void OnScoreChanged(int32 NewScore, int32 DeltaScore, int32 NewMultiplier);
    void OnPopUpTypeChanged(EPopUpType NewPopUpType) { PopUpType = NewPopUpType; }

    FText FormatPopUpScore(int32 DeltaScore, int32 NewMultiplier);
};
