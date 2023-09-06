// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGTextUserWidget.generated.h"

class UTextBlock;

UCLASS()
class CUBE_GAME_API UCGTextUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetText(const FText& NewText);
    void SetColor(const FLinearColor& NewColor);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FText Text;

    virtual void NativePreConstruct() override;
};
