// Cube_Game, All rights reserved.

#include "UI/CGAnimatedUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "CGGameModeBase.h"

void UCGAnimatedUserWidget::ShowStartupAnimation()
{
    PlayAnimation(StartupAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), StartupSound);
}

void UCGAnimatedUserWidget::ShowFadeoutAnimation()
{
    PlayAnimation(FadeoutAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), FadeoutSound);
}

ACGGameModeBase* UCGAnimatedUserWidget::GetGameModeBase() const
{
    return GetWorld()->GetAuthGameMode<ACGGameModeBase>();
}

void UCGAnimatedUserWidget::SetGameState(EGameState NewGameState)
{
    const auto GameMode = GetGameModeBase();
    if (!GameMode)
        return;

    GameMode->SetGameState(NewGameState);
}
