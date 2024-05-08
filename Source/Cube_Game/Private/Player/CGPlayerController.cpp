// Cube_Game, All rights reserved.

#include "Player/CGPlayerController.h"
#include "CGGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CGGameInstance.h"
#include "CGUtils.h"

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

    check(InputMapping);
    check(EnterAction);
    check(EscapeAction);
    check(MoveLeftAction);
    check(MoveRightAction);
    check(UseCurrentBonusAction);

    if (auto* GameModeBase = GetGameModeBase())
    {
        GameModeBase->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (auto* GameInstance = GetWorld()->GetGameInstance<UCGGameInstance>())
    {
        using namespace CubeGame;
        const auto MoveLeftKeyName = Utils::GetActionKeyName(InputMapping, MoveLeftAction).ToUpper();
        const auto MoveRightKeyName = Utils::GetActionKeyName(InputMapping, MoveRightAction).ToUpper();
        const auto UseCurrentBonusKeyName = Utils::GetActionKeyName(InputMapping, UseCurrentBonusAction).ToUpper();
        GameInstance->FormatHints(MoveLeftKeyName, MoveRightKeyName, UseCurrentBonusKeyName);
    }
}

ACGGameModeBase* ACGPlayerController::GetGameModeBase() const
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
