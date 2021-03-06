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
    UTextBlock* ScoreTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* TimeTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* SpeedTextBlock;

    virtual void NativeOnInitialized() override;

private:
    void OnScoreChanged(int32 NewScore, int32 DeltaScore = 0, int32 NewMultiplier = 1);
    void OnTimeChanged(int32 NewTime);
    void OnSpeedChanged(int32 NewSpeed);
};
