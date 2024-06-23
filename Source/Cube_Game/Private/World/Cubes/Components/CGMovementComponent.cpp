// Cube_Game, All rights reserved.

#include "World/Cubes/Components/CGMovementComponent.h"
#include "CGUtils.h"

constexpr static float MovingTeimerRate{0.007f};

UCGMovementComponent::UCGMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCGMovementComponent::SetSpeedData(const FSpeedData& SpeedData)
{
    CurrentSpeedData = SpeedData;
}

void UCGMovementComponent::SetGameSpeed(int32 GameSpeed)
{
    CurrentGameSpeed = GameSpeed;
}

float UCGMovementComponent::GetSpeed() const
{
    return bOverrideSpeed ? Speed : CubeGame::Utils::ComputeCubeSpeed(CurrentSpeedData, CurrentGameSpeed);
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

void UCGMovementComponent::OnMoving()
{
    if (AActor* Owner = GetOwner())
    {
        FVector NewLocation = Owner->GetActorLocation();
        const float Delta = GetSpeed() * MovingTeimerRate;
        Distance += Delta;
        NewLocation.X = NewLocation.X - Delta;
        Owner->SetActorLocation(NewLocation);
    }
}
