// Cube_Game, All rights reserved.

#include "UI/Game/CGHUDUserWidget.h"
#include "CGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UCGHUDUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
    {
        GameMode->OnLowTime.AddUObject(this, &UCGHUDUserWidget::OnLowTime);
    }
}

void UCGHUDUserWidget::OnLowTime()
{
    PlayAnimation(LowTimeAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), LowTimeSound);
}
