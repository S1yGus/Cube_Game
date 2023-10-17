// Cube_Game, All rights reserved.

#include "UI/Game/CGHUDUserWidget.h"
#include "CGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Player/CGPlayerController.h"

void UCGHUDUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ACGGameMode* GameMode = GetGameMode())
    {
        GameMode->OnLowTime.AddUObject(this, &ThisClass::OnLowTime);
    }

    if (auto* PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEscape.AddUObject(this, &ThisClass::OnPressedEscape);
    }
}

ACGGameMode* UCGHUDUserWidget::GetGameMode() const
{
    return GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr;
}

void UCGHUDUserWidget::OnLowTime()
{
    PlayAnimation(LowTimeAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), LowTimeSound);
}

void UCGHUDUserWidget::OnPressedEscape()
{
    if (!IsVisible())
        return;

    ACGGameMode* GameMode = GetGameMode();
    if (!GameMode)
        return;

    GameMode->SetPauseAndChangeGameState(EGameState::Pause);
}
