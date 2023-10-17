// Cube_Game, All rights reserved.

#include "Player/CGPlayerController.h"
#include "CGGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ACGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (const auto* LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(); InputSystem && InputMapping)
        {
            InputSystem->AddMappingContext(InputMapping, 0);
        }
    }

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
        Input->BindAction(EnterAction, ETriggerEvent::Started, this, &ThisClass::OnPressEnter);
        Input->BindAction(EscapeAction, ETriggerEvent::Started, this, &ThisClass::OnPressEscape);
    }
}

void ACGPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (const auto GameModeBase = GetGameModeBase())
    {
        GameModeBase->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }
}

TObjectPtr<ACGGameModeBase> ACGPlayerController::GetGameModeBase() const
{
    return GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameModeBase>() : nullptr;
}

void ACGPlayerController::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState == EGameState::Game)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
        bShowMouseCursor = true;
    }
}

void ACGPlayerController::OnPressEnter()
{
    OnPressedEnter.Broadcast();
}

void ACGPlayerController::OnPressEscape()
{
    OnPressedEscape.Broadcast();
}
