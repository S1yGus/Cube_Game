// Cube_Game, All rights reserved.

#include "Gameplay/CGFieldActor.h"
#include "CGGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "Player/Components/CGBonusComponent.h"
#include "Gameplay/Cubes/Bonuses/CGIndicatorBonusCubeActor.h"
#include "Gameplay/Cubes/CGIndicatorCubeActor.h"
#include "Components/WidgetComponent.h"

ACGFieldActor::ACGFieldActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->CastShadow = false;
    SetRootComponent(StaticMeshComponent);

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawAtDesiredSize(true);
    WidgetComponent->SetupAttachment(StaticMeshComponent);
}

void ACGFieldActor::BeginPlay()
{
    Super::BeginPlay();

    SetupField();

    RestoreSpawnPositions();
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ACGFieldActor::OnSpawnCube, GetSpawnTimerRate(), true);
}

float ACGFieldActor::GetSpawnTimerRate() const
{
    const auto GameMode = GetGameMode();
    const auto DifficultyVlues = GetDifficultyVlues();
    if (!GameMode || !DifficultyVlues)
        return 0.0f;

    return DifficultyVlues->DistanceBetweenCubes / GameMode->GetCubeSpeed();
}

ECubeType ACGFieldActor::GetRandomCubeType() const
{
    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues)
        return ECubeType::None;

    const auto RandNum = FMath::FRand();
    TArray<ECubeType> RandItems;
    for (const auto& ItemPair : DifficultyVlues->SpawnWeightMap)
    {
        if (RandNum > ItemPair.Value)
            continue;

        RandItems.Add(ItemPair.Key);
    }

    if (RandItems.Num() == 0)
        return ECubeType::None;

    return RandItems[FMath::RandHelper(RandItems.Num())];
}

ACGGameMode* ACGFieldActor::GetGameMode() const
{
    return GetWorld()->GetAuthGameMode<ACGGameMode>();
}

const FDifficulty* ACGFieldActor::GetDifficultyVlues() const
{
    return GetGameMode() ? GetGameMode()->GetDifficultyVlues() : nullptr;
}

const APawn* ACGFieldActor::GetPlayerPawn() const
{
    const auto PlayerController = GetWorld()->GetFirstPlayerController();
    return PlayerController ? PlayerController->GetPawn() : nullptr;
}

void ACGFieldActor::SetupField()
{
    if (const auto GameMode = GetGameMode())
    {
        GameMode->OnSpeedChanged.AddUObject(this, &ACGFieldActor::OnSpeedChanged);
        GameMode->OnMultiplierChanged.AddUObject(this, &ACGFieldActor::OnMultiplierChanged);
    }

    if (const auto PlayerPawn = GetPlayerPawn())
    {
        if (const auto BonusComponent = PlayerPawn->FindComponentByClass<UCGBonusComponent>())
        {
            BonusComponent->OnBonusChanged.AddUObject(this, &ACGFieldActor::OnBonusChanged);
        }
    }
}

void ACGFieldActor::OnSpawnCube()
{
    if (++CubesInLine <= GetDifficultyVlues()->MaxNumOfCubesInLine && CubesInLine <= SpawnPositionsAmount)
    {
        const auto RandIndex = FMath::RandHelper(SpawnPositions.Num());
        SpawnCube(SpawnPositions[RandIndex]);
        SpawnPositions.RemoveAt(RandIndex);

        if (UKismetMathLibrary::RandomBoolWithWeight(GetDifficultyVlues()->ChanceToAddCubeInLine))
        {
            OnSpawnCube();
            return;
        }
    }

    if (bRestartSpawn)
    {
        GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ACGFieldActor::OnSpawnCube, GetSpawnTimerRate(), true);
        bRestartSpawn = false;
    }

    RestoreSpawnPositions();
}

void ACGFieldActor::SpawnCube(int32 SpawnPosition)
{
    const FVector RelativeSpawnLocation{SpawnPosition * SpawnStep + SpawnXOffset, SpawnYOffset, SpawnZOffset};
    const auto SpawnTransform = FTransform{RelativeSpawnLocation} * GetTransform();
    const auto SpawnedCube = GetWorld()->SpawnActorDeferred<ACGCubeActor>(SpawningCubeClass, SpawnTransform);
    if (!SpawnedCube)
        return;

    SpawnedCube->SetCubeType(GetRandomCubeType());
    SpawnedCube->FinishSpawning(SpawnTransform);
}

void ACGFieldActor::OnSpeedChanged(int32 NewSpeed)
{
    bRestartSpawn = true;
}

void ACGFieldActor::RestoreSpawnPositions()
{
    CubesInLine = 0;

    SpawnPositions.Empty();
    for (int32 i = 0; i < SpawnPositionsAmount; ++i)
    {
        SpawnPositions.Add(i);
    }
}

void ACGFieldActor::OnMultiplierChanged(ECubeType CubeType, int32 Multiplier)
{
    if (Multiplier == 1 && Indicators.Num() != 0)
    {
        for (const auto& Indicator : Indicators)
        {
            Indicator->Teardown();
        }

        Indicators.Empty();
        return;
    }

    SpawnIndicator(CubeType, Multiplier);
}

void ACGFieldActor::SpawnIndicator(ECubeType CubeType, int32 Multiplier)
{
    const FVector RelativeIndLocation{SpawnPositionsAmount * SpawnStep + SpawnXOffset,                     //
                                      (Multiplier - 2) * -IndicatorsSpawnStep + IndicatorsSpawnYOffset,    //
                                      IndicatorsSpawnZOffset};                                             //
    // (Multiplier - 2) because the first an indicator cube spawn when Multiplier be 2.
    const auto IndicatorSpawnTransform = FTransform{RelativeIndLocation} * GetTransform();
    const auto Indicator = GetWorld()->SpawnActorDeferred<ACGIndicatorCubeActor>(IndicatorClass, IndicatorSpawnTransform);
    if (!Indicator)
        return;

    Indicator->SetCubeType(CubeType);
    Indicator->FinishSpawning(IndicatorSpawnTransform);
    Indicators.Add(Indicator);
}

void ACGFieldActor::OnBonusChanged(EBonusType BonusType)
{
    NewBonusIndicatorType = BonusType;

    if (IsValid(BonusIndicator))
    {
        BonusIndicator->Teardown();
    }
    else
    {
        SpawnBonusIndicator(BonusType);
    }

    ChangeFieldColor(BonusType);
}

void ACGFieldActor::ChangeFieldColor(EBonusType BonusType)
{
    const auto FieldMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
    if (!FieldMaterial)
        return;

    if (!MaterialColorsMap.Contains(BonusType))
    {
        FieldMaterial->SetVectorParameterValue(ColorParamName, DefaultColor);
        return;
    }

    FieldMaterial->SetVectorParameterValue(ColorParamName, MaterialColorsMap[BonusType]);
}

void ACGFieldActor::SpawnBonusIndicator(EBonusType BonusType)
{
    const FVector RelativeIndLocation{SpawnPositionsAmount * SpawnStep + SpawnXOffset,                           //
                                      BonusIndicatorPosition * -IndicatorsSpawnStep + IndicatorsSpawnYOffset,    //
                                      IndicatorsSpawnZOffset};                                                   //
    const auto IndicatorSpawnTransform = FTransform{RelativeIndLocation} * GetTransform();
    BonusIndicator = GetWorld()->SpawnActorDeferred<ACGIndicatorBonusCubeActor>(BonusIndicatorClass, IndicatorSpawnTransform);
    if (!BonusIndicator)
        return;

    BonusIndicator->SetBonusType(BonusType);
    BonusIndicator->OnDestroyed.AddDynamic(this, &ACGFieldActor::OnBonusIndicatorDestroyed);
    BonusIndicator->FinishSpawning(IndicatorSpawnTransform);
}

void ACGFieldActor::OnBonusIndicatorDestroyed(AActor* DestroyedActor)
{
    if (NewBonusIndicatorType == EBonusType::None)
        return;

    SpawnBonusIndicator(NewBonusIndicatorType);
}
