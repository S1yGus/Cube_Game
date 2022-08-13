// Cube_Game, All rights reserved.

#include "Gameplay/CGFieldActor.h"
#include "CGGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Cubes/CGBaseCubeActor.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "Player/Components/CGBonusComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ACGFieldActor::ACGFieldActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->CastShadow = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawAtDesiredSize(true);
    WidgetComponent->SetupAttachment(StaticMeshComponent);

    SetRootComponent(StaticMeshComponent);
}

void ACGFieldActor::BeginPlay()
{
    Super::BeginPlay();

    SetupField();

    RestoreSpawnPositions();
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::OnSpawnCube, GetSpawnTimerRate(), true);
}

float ACGFieldActor::GetSpawnTimerRate() const
{
    const auto GameMode = GetGameMode();
    if (!GameMode)
        return 0.0f;

    return GetDifficultyVlues().DistanceBetweenCubes / GameMode->GetCubeSpeed();
}

ECubeType ACGFieldActor::GetRandomCubeType() const
{
    const auto DifficultyVlues = GetDifficultyVlues();
    if (!DifficultyVlues.SpawnWeightMap.Num())
        return ECubeType::None;

    const auto RandNum = FMath::FRand();
    TArray<ECubeType> RandItems;
    for (const auto& ItemPair : DifficultyVlues.SpawnWeightMap)
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

const FDifficulty& ACGFieldActor::GetDifficultyVlues() const
{
    if (const auto GameMode = GetGameMode())
    {
        return GameMode->GetDifficultyVlues();
    }

    static const auto Difficulty = FDifficulty{};
    return Difficulty;
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
        GameMode->OnSpeedChanged.AddUObject(this, &ThisClass::OnSpeedChanged);
        GameMode->OnMultiplierChanged.AddUObject(this, &ThisClass::OnMultiplierChanged);
    }

    if (const auto PlayerPawn = GetPlayerPawn())
    {
        if (const auto BonusComponent = PlayerPawn->FindComponentByClass<UCGBonusComponent>())
        {
            BonusComponent->OnBonusChanged.AddUObject(this, &ThisClass::OnBonusChanged);
        }
    }

    ChangeFieldColor(EBonusType::None);    // Set default field color.
}

void ACGFieldActor::OnSpawnCube()
{
    if (++CubesInLine <= GetDifficultyVlues().MaxNumOfCubesInLine && CubesInLine <= SpawnPositionsAmount)
    {
        const auto RandIndex = FMath::RandHelper(SpawnPositions.Num());
        SpawnCube(SpawnPositions[RandIndex]);
        SpawnPositions.RemoveAt(RandIndex);

        if (UKismetMathLibrary::RandomBoolWithWeight(GetDifficultyVlues().ChanceToAddCubeInLine))
        {
            OnSpawnCube();
            return;
        }
    }

    if (bRestartSpawn)    // Spawn is restarting after speed have been changed.
    {
        GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::OnSpawnCube, GetSpawnTimerRate(), true);
        bRestartSpawn = false;
    }

    RestoreSpawnPositions();
}

void ACGFieldActor::SpawnCube(int32 SpawnPosition)
{
    const FVector RelativeLocation{SpawnPosition * SpawnStep + SpawnXOffset, SpawnYOffset, SpawnZOffset};
    const auto CubeType = GetRandomCubeType();
    const auto CubeColorData = CubeColorDataMap.Contains(CubeType) ? CubeColorDataMap[CubeType] : FCubeColorData{};
    auto SpawnedCube = SpawnCubeActor<ACGCubeActor>(SpawningCubeClass, RelativeLocation, CubeColorData);
    SpawnedCube->SetCubeType(CubeType);
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
    // (Multiplier - 2) because the first an indicator cube spawn when Multiplier be 2.
    const FVector RelativeLocation{SpawnPositionsAmount * SpawnStep + SpawnXOffset,                     //
                                   (Multiplier - 2) * -IndicatorsSpawnStep + IndicatorsSpawnYOffset,    //
                                   IndicatorsSpawnZOffset};                                             //
    const auto CubeColorData = CubeColorDataMap.Contains(CubeType) ? CubeColorDataMap[CubeType] : FCubeColorData{};
    const auto Indicator = SpawnCubeActor<ACGBaseCubeActor>(IndicatorClass, RelativeLocation, CubeColorData);
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
    if (!MaterialColorsMap.Contains(BonusType))
        return;

    const auto DynMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
    check(DynMaterial);
    DynMaterial->SetVectorParameterValue(ColorParamName, MaterialColorsMap[BonusType]);
}

void ACGFieldActor::SpawnBonusIndicator(EBonusType BonusType)
{
    const FVector RelativeLocation{SpawnPositionsAmount * SpawnStep + SpawnXOffset,                           //
                                   BonusIndicatorPosition * -IndicatorsSpawnStep + IndicatorsSpawnYOffset,    //
                                   IndicatorsSpawnZOffset};                                                   //
    const auto CubeColorData = BonusColorDataMap.Contains(BonusType) ? BonusColorDataMap[BonusType] : FCubeColorData{};
    BonusIndicator = SpawnCubeActor<ACGBaseCubeActor>(IndicatorClass, RelativeLocation, CubeColorData);
    BonusIndicator->OnDestroyed.AddDynamic(this, &ThisClass::OnBonusIndicatorDestroyed);
}

void ACGFieldActor::OnBonusIndicatorDestroyed(AActor* DestroyedActor)
{
    if (NewBonusIndicatorType == EBonusType::None)
        return;

    SpawnBonusIndicator(NewBonusIndicatorType);
}
