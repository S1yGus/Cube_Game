// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGAnimatedUserWidget.h"
#include "CGCoreTypes.h"
#include "CGOptionsWarningUserWidget.generated.h"

class UCGButtonUserWidget;

UCLASS()
class CUBE_GAME_API UCGOptionsWarningUserWidget : public UCGAnimatedUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FText GetCountdownTime() const;

protected:
    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> SaveButton;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCGButtonUserWidget> CancelButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float CancelSettingsTime{10.0f};

    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    UPROPERTY()
    TArray<TObjectPtr<UCGButtonUserWidget>> WidgetButtons;

    float CountdownTime{0.0f};

    void Setup();
    void ResetWidget();

    void OnGameStateChanged(EGameState NewGameState);
    void OnPressedEnter();
    void OnPressedEsc();
    void OnSaveSettings();
    void OnCancelSettings();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
