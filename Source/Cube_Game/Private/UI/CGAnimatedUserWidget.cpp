// Cube_Game, All rights reserved.

#include "UI/CGAnimatedUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "CGGameModeBase.h"

void UCGAnimatedUserWidget::ShowStartupAnimation()
{
    PlayAnimation(StartupAnimation);
}

void UCGAnimatedUserWidget::ShowFadeoutAnimation()
{
    PlayAnimation(FadeoutAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), FadeoutSound);
}

void UCGAnimatedUserWidget::TransitionToGameState(EGameState GameState)
{
    GameStateToSet = GameState;
    ShowFadeoutAnimation();
}

TObjectPtr<ACGGameModeBase> UCGAnimatedUserWidget::GetGameModeBase() const
{
    return GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameModeBase>() : nullptr;
}

void UCGAnimatedUserWidget::SetGameState(EGameState NewGameState)
{
    if (TObjectPtr<ACGGameModeBase> GameMode = GetGameModeBase())
    {
        GameMode->SetGameState(NewGameState);
    }
}

void UCGAnimatedUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == StartupAnimation)
    {
        OnStartupAnimationFinished();
    }
    else if (Animation == FadeoutAnimation)
    {
        OnFadeoutAnimationFinished();
    }
}

void UCGAnimatedUserWidget::OnStartupAnimationFinished()
{
    UGameplayStatics::PlaySound2D(GetWorld(), StartupSound);
}

void UCGAnimatedUserWidget::OnFadeoutAnimationFinished()
{
    SetGameState(GameStateToSet);
}
