// Cube_Game, All rights reserved.

#include "Player/CGPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Player/Components/CGBonusComponent.h"
#include "Player/Components/CGFXComponent.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "CGGameMode.h"
#include "Settings/CGGameUserSettings.h"

constexpr static float MovementTimerRate{0.016f};
constexpr static float MovementSpeed{10.0f};

ACGPlayer::ACGPlayer()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

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
}

void ACGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Right", EInputEvent::IE_Pressed, this, &ThisClass::MoveRight);
    PlayerInputComponent->BindAction("Left", EInputEvent::IE_Pressed, this, &ThisClass::MoveLeft);
    PlayerInputComponent->BindAction("UseBonus", EInputEvent::IE_Pressed, this, &ThisClass::UseCurrentBonus);
}

FVector ACGPlayer::GetCurrentPositionLocation() const
{
    return FVector{CurrentPosition * MovementStep, 0.0, PositionZOffset};
}

void ACGPlayer::SetupPlayer()
{
    FXComponent->SetCollectColor(ECubeType::None);    // Set default player color.
    CurrentPosition = FMath::RandHelper(PositionsAmount);

    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
    BonusComponent->OnBonusCharged.AddUObject(FXComponent, &UCGFXComponent::OnBonusCharged);

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->OnHintsStatusChanged.AddUObject(this, &ThisClass::OnHintsStatusChanged);
        CachedCollectHintsMap = GameUserSettings->GetHintsStatus().CollectHintsMap;
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
    GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &ThisClass::OnMoving, MovementTimerRate, true);
}

void ACGPlayer::OnMoving()
{
    const FVector NewLocation = FMath::VInterpTo(StaticMeshComponent->GetComponentLocation(), GetCurrentPositionLocation(), MovementTimerRate, MovementSpeed);
    StaticMeshComponent->SetWorldLocation(NewLocation);

    if (StaticMeshComponent->GetComponentLocation().Equals(GetCurrentPositionLocation()))
    {
        GetWorldTimerManager().ClearTimer(MovementTimerHandle);
    }
}

void ACGPlayer::UseCurrentBonus()
{
    FXComponent->MakeCameraShake(BonusComponent->GetCurrentBonusType());
    BonusComponent->UseCurrentBonus();
}

void ACGPlayer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                        AActor* OtherActor,                          //
                                        UPrimitiveComponent* OtherComp,              //
                                        int32 OtherBodyIndex,                        //
                                        bool bFromSweep,                             //
                                        const FHitResult& SweepResult)               //
{
    if (auto* OverlapedCube = Cast<ACGCubeActor>(OtherActor))
    {
        OverlapedCube->Collect();
        CollectCube(OverlapedCube->GetCubeType());
    }
}

void ACGPlayer::CollectCube(ECubeType CubeType)
{
    FXComponent->PlayCollectSound(CubeType);
    FXComponent->SetCollectColor(CubeType);
    FXComponent->MakeCameraShake(CubeType);

    ShowPopUpHint(CubeType);

    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
    {
        GameMode->ChangeTime(CubeType);
        GameMode->ChangeSpeed(CubeType);
        GameMode->ChangeScore(CubeType);
    }

    if (CubeType == ECubeType::BonusCube)
    {
        BonusComponent->CollectBonusCube();
    }
}

void ACGPlayer::ShowPopUpHint(ECubeType CubeType)
{
    if (!CachedCollectHintsMap.Contains(CubeType))
        return;

    if (!CachedCollectHintsMap[CubeType])
        return;

    auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr;
    if (!GameMode || !GameMode->GetCollectHints().Contains(CubeType))
        return;

    GameMode->ShowPopUpHint(GameMode->GetCollectHints()[CubeType]);

    CachedCollectHintsMap[CubeType] = false;

    if (auto* GameUserSettings = UCGGameUserSettings::Get())
    {
        GameUserSettings->SetCollectHintsStatus(CachedCollectHintsMap);
    }
}

void ACGPlayer::OnHintsStatusChanged(const FHintsStatus& NewHintsStatus)
{
    CachedCollectHintsMap = NewHintsStatus.CollectHintsMap;
}
