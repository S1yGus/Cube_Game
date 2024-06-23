// Cube_Game, All rights reserved.

#include "World/Cubes/Bonuses/CGShieldBonusActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ACGShieldBonusActor::Teardown()
{
    Super::Teardown();

    UGameplayStatics::PlaySound2D(GetWorld(), TeardownSound);
}

void ACGShieldBonusActor::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(ShieldTimerHandle, this, &ACGShieldBonusActor::Teardown, bCharged ? ChargedShieldDuration : ShieldDuration);
}

void ACGShieldBonusActor::OnScaling()
{
    Super::OnScaling();

    // To avoid situations when cubes are collected by mistake.
    if (GetActorScale3D().X >= ActivationScale.X        //
        && GetActorScale3D().Y >= ActivationScale.Y     //
        && GetActorScale3D().Z >= ActivationScale.Z)    //
    {
        SetPlayerMeshCollisionEnabled(false);
    }
    else if (GetActorScale3D().X < ActivationScale.X        //
             && GetActorScale3D().Y < ActivationScale.Y     //
             && GetActorScale3D().Z < ActivationScale.Z)    //
    {
        SetPlayerMeshCollisionEnabled(true);
    }
}

void ACGShieldBonusActor::SetPlayerMeshCollisionEnabled(bool bEnabled)
{
    if (UStaticMeshComponent* PlayerMesh = GetPlayerMesh())
    {
        PlayerMesh->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
    }
}
