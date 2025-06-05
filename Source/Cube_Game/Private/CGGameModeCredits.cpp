// Cube_Game, All rights reserved.

#include "CGGameModeCredits.h"
#include "CGGameInstance.h"
#include "Player/CGPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/CGHUDCredits.h"

ACGGameModeCredits::ACGGameModeCredits()
{
    static ConstructorHelpers::FClassFinder<ACGHUDCredits> HUD(TEXT("/Game/Framework/BP_HUDCredits"));
    if (HUD.Class)
    {
        HUDClass = HUD.Class;
    }
}

void ACGGameModeCredits::StartPlay()
{
    Super::StartPlay();

    SetGameState(EGameState::Authors);

    if (auto* PC = GetWorld()->GetFirstPlayerController<ACGPlayerController>())
    {
        PC->OnPressedEscape.AddUObject(this, &ThisClass::OnPressedEscape);
    }
}

void ACGGameModeCredits::OnPressedEscape()
{
    if (auto* GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>())
    {
        switch (CurrentGameState)
        {
            case EGameState::Authors:
                SetGameState(EGameState::Title);
                break;
            case EGameState::Title:
                GameInstance->OpenMainMenu();
                break;
            default:
                GameInstance->OpenMainMenu();
                break;
        }
    }
}