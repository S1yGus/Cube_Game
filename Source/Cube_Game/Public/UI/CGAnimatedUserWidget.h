// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGCoreTypes.h"
#include "CGAnimatedUserWidget.generated.h"

class USoundCue;
class ACGGameModeBase;

UCLASS()
class CUBE_GAME_API UCGAnimatedUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void ShowStartupAnimation();
    void ShowFadeoutAnimation();

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> StartupAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> StartupSound;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> FadeoutAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> FadeoutSound;

    EGameState GameStateToSet{EGameState::WaitingToStart};

    void TransitionToGameState(EGameState GameState);
    TObjectPtr<ACGGameModeBase> GetGameModeBase() const;

    virtual void SetGameState(EGameState NewGameState);

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
    virtual void OnStartupAnimationFinished();
    virtual void OnFadeoutAnimationFinished();
};
