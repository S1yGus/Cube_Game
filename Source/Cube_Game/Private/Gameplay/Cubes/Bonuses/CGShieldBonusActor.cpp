// Cube_Game, All rights reserved.

#include "Gameplay/Cubes/Bonuses/CGShieldBonusActor.h"
#include "Player/CGPlayer.h"
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

    GetWorldTimerManager().SetTimer(ShieldTimerHandle, this, &ACGShieldBonusActor::Teardown, ShieldDuration);
}

void ACGShieldBonusActor::OnScalin()
{
    // To avoid situations when cubes are received by mistake.
    if (bCanOffPlayerCollision                          //
        && GetActorScale3D().X >= ActivationScale.X     //
        && GetActorScale3D().Y >= ActivationScale.Y     //
        && GetActorScale3D().Z >= ActivationScale.Z)    //
    {
        SetPlayerMeshCollision(false);
        bCanOffPlayerCollision = false;
    }
    else if (!bCanOffPlayerCollision                        //
             && GetActorScale3D().X < ActivationScale.X     //
             && GetActorScale3D().Y < ActivationScale.Y     //
             && GetActorScale3D().Z < ActivationScale.Z)    //
    {
        SetPlayerMeshCollision(true);
        bCanOffPlayerCollision = true;
    }
}

void ACGShieldBonusActor::SetPlayerMeshCollision(bool CollisionEnabled)
{
    const auto PlayerMesh = GetPlayerMesh();
    if (!PlayerMesh)
        return;

    PlayerMesh->SetCollisionEnabled(CollisionEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}
