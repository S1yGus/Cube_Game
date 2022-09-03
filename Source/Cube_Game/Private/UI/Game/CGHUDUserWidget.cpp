// Cube_Game, All rights reserved.

#include "UI/Game/CGHUDUserWidget.h"
#include "CGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Player/CGPlayerController.h"

void UCGHUDUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (const auto GameMode = GetGameMode())
    {
        GameMode->OnLowTime.AddUObject(this, &ThisClass::OnLowTime);
    }

    if (const auto PC = GetOwningPlayer<ACGPlayerController>())
    {
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEsc);
    }
}

inline ACGGameMode* UCGHUDUserWidget::GetGameMode() const
{
    return GetWorld()->GetAuthGameMode<ACGGameMode>();
}

void UCGHUDUserWidget::OnLowTime()
{
    PlayAnimation(LowTimeAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), LowTimeSound);
}

void UCGHUDUserWidget::OnPressedEsc()
{
    if (!IsVisible())
        return;

    const auto GameMode = GetGameMode();
    if (!GameMode)
        return;

    GameMode->SetPause(EGameState::Pause);
}
