// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGInfoUserWidget.generated.h"

class UTextBlock;

UCLASS()
class CUBE_GAME_API UCGInfoUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> ScoreTextBlock;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> TimeTextBlock;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> SpeedTextBlock;

    virtual void NativeOnInitialized() override;

private:
    void Setup();

    void OnScoreChanged(int32 NewScore, int32 DeltaScore = 0, int32 NewMultiplier = 1);
    void OnTimeChanged(int32 NewTime);
    void OnSpeedChanged(int32 NewSpeed);
};
