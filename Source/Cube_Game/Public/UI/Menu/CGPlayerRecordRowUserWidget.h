// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGPlayerRecordRowUserWidget.generated.h"

class UCGTextUserWidget;

UCLASS()
class CUBE_GAME_API UCGPlayerRecordRowUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetNameText(const FText& NewText);
    void SetScoreText(const FText& NewText);
    void SetDateText(const FText& NewText);

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGTextUserWidget> NameText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGTextUserWidget> ScoreText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGTextUserWidget> DateText;

    virtual void NativeOnInitialized() override;
};
