// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Components/CGMovementComponent.h"
#include "CGGameMode.h"

constexpr static float MovingTeimerRate{0.007f};

UCGMovementComponent::UCGMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGMovementComponent::StopMoving()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
    }
}

void UCGMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &ThisClass::OnMoving, MovingTeimerRate, true);
    }
}

int32 UCGMovementComponent::GetCubeSpeed() const
{
    if (const auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
    {
        return bOverrideGameModeSpeed ? Speed : GameMode->GetCubeSpeed();
    }

    return Speed;
}

void UCGMovementComponent::OnMoving()
{
    if (AActor* Owner = GetOwner())
    {
        FVector NewLocation = Owner->GetActorLocation();
        NewLocation.X = NewLocation.X - GetCubeSpeed() * MovingTeimerRate;
        Owner->SetActorLocation(NewLocation);
    }
}
