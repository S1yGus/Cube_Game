// Cube_Game, All rights reserved.

#include "Player/CGPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/Components/CGBonusComponent.h"
#include "Player/Components/CGFXComponent.h"
#include "World/Cubes/CGCubeActor.h"
#include "CGGameMode.h"
#include "EnhancedInputComponent.h"
#include "CGUtils.h"

constexpr static float MovementTimerRate{0.016f};
constexpr static float MovementSpeed{10.0f};

static double HalfFOVTan(double FOV)
{
    return FMath::Tan(FMath::DegreesToRadians(FOV * 0.5));
}

// https://en.wikipedia.org/wiki/Field_of_view_in_video_games
static double VerticalFOV(double HorizontalFOV, double AspectRatioHW)
{
    return 2.0 * FMath::RadiansToDegrees(FMath::Atan(HalfFOVTan(HorizontalFOV) * AspectRatioHW));
}

ACGPlayer::ACGPlayer()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    check(StaticMeshComponent);
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
    check(WidgetComponent);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawAtDesiredSize(true);
    WidgetComponent->SetupAttachment(StaticMeshComponent);

    BonusComponent = CreateDefaultSubobject<UCGBonusComponent>("BonusComponent");
    check(BonusComponent);

    FXComponent = CreateDefaultSubobject<UCGFXComponent>("FXComponent");
    check(FXComponent);
}

void ACGPlayer::UpdateLocation(const FTransform& InOrigin, const FVector& InFieldSize)
{
    Origin = InOrigin;
    FieldSize = InFieldSize;

    if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
    {
        FViewport* Viewport = GEngine->GameViewport->Viewport;
        Viewport->ViewportResizedEvent.Remove(OnViewportResizedHandle);
        OnViewportResizedHandle = Viewport->ViewportResizedEvent.AddUObject(this, &ThisClass::OnViewportResized);

        OnViewportResized(Viewport, 0);
    }
}

void ACGPlayer::BeginPlay()
{
    Super::BeginPlay();

    check(StaticMeshComponent->GetStaticMesh());

    SetupPlayer();
}

void ACGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(UseCurrentBonusAction, ETriggerEvent::Started, this, &ThisClass::OnUseCurrentBonus);
        Input->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ThisClass::OnMoveRight);
        Input->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ThisClass::OnMoveLeft);
    }
}

FVector ACGPlayer::GetTargetPositionLocation() const
{
    return Origin.GetLocation() + FVector{PositionXOffset, CurrentPosition * MovementStep + PositionYOffset, PositionZOffset};
}

void ACGPlayer::SetupPlayer()
{
    FXComponent->SetCollectColor(ECubeType::None);    // Set default player color.
    CurrentPosition = FMath::RandHelper(PositionsAmount);

    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPlayerBeginOverlap);
    BonusComponent->OnBonusCharged.AddUObject(FXComponent, &UCGFXComponent::OnBonusCharged);
    BonusComponent->OnBonusBeginOverlap.AddUObject(this, &ACGPlayer::OnBonusBeginOverlap);
}

void ACGPlayer::OnUseCurrentBonus()
{
    FXComponent->MakeCameraShake(BonusComponent->GetCurrentBonusType());
    BonusComponent->UseCurrentBonus();
}

void ACGPlayer::OnMoveRight()
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

    StartMovingToCurrentPosition();
}

void ACGPlayer::OnMoveLeft()
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

    StartMovingToCurrentPosition();
}

void ACGPlayer::StartMovingToCurrentPosition()
{
    GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &ThisClass::OnMoving, MovementTimerRate, true);
}

void ACGPlayer::OnMoving()
{
    const FVector NewLocation = FMath::VInterpTo(StaticMeshComponent->GetComponentLocation(), GetTargetPositionLocation(), MovementTimerRate, MovementSpeed);
    StaticMeshComponent->SetWorldLocation(NewLocation);

    if (StaticMeshComponent->GetComponentLocation().Equals(GetTargetPositionLocation()))
    {
        GetWorldTimerManager().ClearTimer(MovementTimerHandle);
    }
}

void ACGPlayer::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                     AActor* OtherActor,                          //
                                     UPrimitiveComponent* OtherComp,              //
                                     int32 OtherBodyIndex,                        //
                                     bool bFromSweep,                             //
                                     const FHitResult& SweepResult)               //
{
    if (auto* Cube = Cast<ACGCubeActor>(OtherActor))
    {
        CollectCube(Cube);
    }
}

void ACGPlayer::OnBonusBeginOverlap(ACGCubeActor* Cube, bool bCharged)
{
    if (const auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
    {
        if (bCharged && !CubeGame::Utils::IsCubeNegative(Cube->GetCubeType(), GameMode->GetDifficultyData()))
        {
            CollectCube(Cube);
        }
        else
        {
            Cube->Annihilate();
        }
    }
}

void ACGPlayer::OnViewportResized(FViewport* Viewport, uint32 Value)
{
    if (!Viewport || Viewport->GetSizeXY().X == 0 || Viewport->GetSizeXY().Y == 0 || FieldSize.Y == 0)
        return;

    double ZLocation{0.0};
    const double ViewportAspect = static_cast<double>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const double FieldAspect = FieldSize.Y / FieldSize.X;
    if (FieldAspect < ViewportAspect)
    {
        const uint32 MarginHeight = FieldSize.X + FieldMargin;
        ZLocation = MarginHeight / HalfFOVTan(VerticalFOV(CameraComponent->FieldOfView, 1.0 / ViewportAspect));
    }
    else
    {
        const uint32 MarginWidth = FieldSize.Y + FieldMargin;
        ZLocation = MarginWidth / HalfFOVTan(CameraComponent->FieldOfView);
    }
    SetActorLocation(Origin.GetLocation() + 0.5 * FVector{FieldSize.X, FieldSize.Y, ZLocation});
    StaticMeshComponent->SetWorldLocation(GetTargetPositionLocation());
}

void ACGPlayer::ProduceCollectEffects(ECubeType CubeType)
{
    FXComponent->PlayCollectSound(CubeType);
    FXComponent->SetCollectColor(CubeType);
    FXComponent->MakeCameraShake(CubeType);
}

void ACGPlayer::UpdateGameMode(ECubeType CubeType)
{
    if (auto* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr)
    {
        GameMode->UpdateGameMetrics(CubeType);
        GameMode->EnqueueHint(CubeType);
    }
}

void ACGPlayer::CollectCube(ACGCubeActor* Cube)
{
    const auto CubeType{Cube->GetCubeType()};
    ProduceCollectEffects(CubeType);
    UpdateGameMode(CubeType);
    if (CubeType == ECubeType::BonusCube)
    {
        BonusComponent->CollectBonusCube();
    }
    Cube->Collect();
}
