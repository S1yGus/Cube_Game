// Cube_Game, All rights reserved.

#include "Player/CGPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Player/Components/CGBonusComponent.h"
#include "Player/Components/CGFXComponent.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "CGGameMode.h"
#include "Settings/CGGameUserSettings.h"

constexpr static float MovementTimerRate = 0.016f;
constexpr static float MovementSpeed = 10.0f;

ACGPlayer::ACGPlayer()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(CameraComponent);

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawAtDesiredSize(true);
    WidgetComponent->SetupAttachment(StaticMeshComponent);

    BonusComponent = CreateDefaultSubobject<UCGBonusComponent>("BonusComponent");
    FXComponent = CreateDefaultSubobject<UCGFXComponent>("FXComponent");
}

void ACGPlayer::BeginPlay()
{
    Super::BeginPlay();

    check(StaticMeshComponent);
    check(WidgetComponent);
    check(BonusComponent);
    check(FXComponent);

    SetupPlayer();
    MoveToCurrentPosition();

    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACGPlayer::OnComponentBeginOverlap);
}

void ACGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Right", EInputEvent::IE_Pressed, this, &ACGPlayer::MoveRight);
    PlayerInputComponent->BindAction("Left", EInputEvent::IE_Pressed, this, &ACGPlayer::MoveLeft);
    PlayerInputComponent->BindAction("UseBonus", EInputEvent::IE_Pressed, BonusComponent, &UCGBonusComponent::UseCurrentBonus);
}

FVector ACGPlayer::GetCurrentPositionLocation() const
{
    return FVector{CurrentPosition * MovementStep, 0.0, PositionZOffset};
}

void ACGPlayer::SetupPlayer()
{
    CurrentPosition = FMath::RandHelper(PositionsAmount);

    if (const auto GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->OnHintsStatusChanged.AddUObject(this, &ThisClass::OnHintsStatusChanged);
        ReceivingHintsMap = GameUserSettings->GetHintsStatus().ReceivingHintsMap;
    }
}

void ACGPlayer::MoveRight()
{
    if (CurrentPosition + 1 >= PositionsAmount)
    {
        CurrentPosition = PositionsAmount - 1;
        FXComponent->OutOfPosition();
    }
    else
    {
        ++CurrentPosition;
    }

    MoveToCurrentPosition();
}

void ACGPlayer::MoveLeft()
{
    if (CurrentPosition - 1 < 0)
    {
        CurrentPosition = 0;
        FXComponent->OutOfPosition();
    }
    else
    {
        --CurrentPosition;
    }

    MoveToCurrentPosition();
}

void ACGPlayer::MoveToCurrentPosition()
{
    GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &ACGPlayer::OnMoving, MovementTimerRate, true);
}

void ACGPlayer::OnMoving()
{
    const auto NewLocation = FMath::VInterpTo(StaticMeshComponent->GetComponentLocation(), GetCurrentPositionLocation(), MovementTimerRate, MovementSpeed);
    StaticMeshComponent->SetWorldLocation(NewLocation);

    if (FMath::IsNearlyEqual(StaticMeshComponent->GetComponentLocation().X, GetCurrentPositionLocation().X)        //
        && FMath::IsNearlyEqual(StaticMeshComponent->GetComponentLocation().Y, GetCurrentPositionLocation().Y)     //
        && FMath::IsNearlyEqual(StaticMeshComponent->GetComponentLocation().Z, GetCurrentPositionLocation().Z))    //
    {
        GetWorldTimerManager().ClearTimer(MovementTimerHandle);
    }
}

void ACGPlayer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                        AActor* OtherActor,                          //
                                        UPrimitiveComponent* OtherComp,              //
                                        int32 OtherBodyIndex,                        //
                                        bool bFromSweep,                             //
                                        const FHitResult& SweepResult)               //
{
    const auto OverlapedCube = Cast<ACGCubeActor>(OtherActor);
    if (!OverlapedCube)
        return;

    ReceiveCube(OverlapedCube->GetCubeType());
    OverlapedCube->Teardown();
}

void ACGPlayer::ReceiveCube(ECubeType CubeType)
{
    FXComponent->PlayReceivingSound(CubeType);
    FXComponent->SetReceivingMaterial(CubeType);
    FXComponent->MakeCameraShake(CubeType);

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>())
    {
        GameMode->ChangeTime(CubeType);
        GameMode->ChangeSpeed(CubeType);
        GameMode->ChangeScore(CubeType);
    }

    if (CubeType == ECubeType::BonusCube)
    {
        BonusComponent->SetRandomBonus();
    }
    else if (CubeType == ECubeType::PowerUpCube)
    {
        BonusComponent->UseRandomPowerup();
    }

    ShowPopUpHint(CubeType);
}

void ACGPlayer::ShowPopUpHint(ECubeType CubeType)
{
    if (!ReceivingHintsMap.Contains(CubeType))
        return;

    if (!ReceivingHintsMap[CubeType])
        return;

    const auto GameMode = GetWorld()->GetAuthGameMode<ACGGameMode>();
    if (!GameMode || !GameMode->GetReceivingHints().Contains(CubeType))
        return;

    GameMode->ShowPopUpHint(GameMode->GetReceivingHints()[CubeType]);

    ReceivingHintsMap[CubeType] = false;

    if (const auto GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SetReceivingHintsStatus(ReceivingHintsMap);
    }
}

void ACGPlayer::OnHintsStatusChanged(const FHintsStatus& NewHintsStatus)
{
    ReceivingHintsMap = NewHintsStatus.ReceivingHintsMap;
}
