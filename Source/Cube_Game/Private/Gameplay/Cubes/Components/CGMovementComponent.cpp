// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Components/CGMovementComponent.h"
#include "CGGameMode.h"

constexpr static float MovingTeimerRate = 0.007f;

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
    const auto GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr;
    if (!GameMode)
        return Speed;

    return bGetSpeedFromGameMode ? GameMode->GetCubeSpeed() : Speed;
}

void UCGMovementComponent::OnMoving()
{
    const auto Owner = GetOwner();
    if (!Owner)
        return;

    auto NewLocation = Owner->GetActorLocation();
    NewLocation.Y = NewLocation.Y + GetCubeSpeed() * MovingTeimerRate;
    Owner->SetActorLocation(NewLocation);
}
