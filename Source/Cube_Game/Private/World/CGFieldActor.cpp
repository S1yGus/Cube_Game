// Cube_Game, All rights reserved.

#include "World/CGFieldActor.h"
#include "World/Cubes/CGBaseCubeActor.h"
#include "World/Cubes/CGCubeActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "CGUtils.h"

ACGFieldActor::ACGFieldActor()
{
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    check(StaticMeshComponent);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->CastShadow = false;
    SetRootComponent(StaticMeshComponent);

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
    check(WidgetComponent);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    WidgetComponent->SetDrawAtDesiredSize(true);
    WidgetComponent->SetCastShadow(false);
    WidgetComponent->SetupAttachment(GetRootComponent());
}

void ACGFieldActor::Init(const FDifficulty& DifficultyData, int32 GameSpeed, int32 MaxMultiplier)
{
    CurrentDifficultyData = DifficultyData;
    CurrentGameSpeed = GameSpeed;
    BonusIndicatorPosition = MaxMultiplier - 1;
}

FVector ACGFieldActor::GetSize() const
{
    const FBox Box = StaticMeshComponent->GetStaticMesh()->GetBoundingBox();
    return {Box.GetSize().X, Box.GetSize().Y + CubesSpawnStep, Box.GetSize().Z};
}

void ACGFieldActor::BeginPlay()
{
    Super::BeginPlay();

    check(StaticMeshComponent->GetStaticMesh());

    ChangeFieldColor(EBonusType::None);    // Set default field color.

    RestoreSpawnPositions();
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::OnSpawnCube, GetSpawnTimerRate(), true);
}

float ACGFieldActor::GetSpawnTimerRate() const
{
    return CurrentDifficultyData.DistanceBetweenCubes / CubeGame::Utils::ComputeCubeSpeed(CurrentDifficultyData.SpeedData, CurrentGameSpeed);
}

ECubeType ACGFieldActor::GetRandomCubeType() const
{
    const float RandNum = FMath::FRand();
    TArray<ECubeType> RandItems;
    for (const auto& [Type, Probability] : CurrentDifficultyData.SpawnWeightMap)
    {
        if (RandNum < Probability)
        {
            RandItems.Add(Type);
        }
    }

    if (!RandItems.IsEmpty())
    {
        return RandItems[FMath::RandHelper(RandItems.Num())];
    }

    return ECubeType::None;
}

void ACGFieldActor::OnSpawnCube()
{
    if (++CubesInLine <= CurrentDifficultyData.MaxNumOfCubesInLine && CubesInLine <= SpawnPositionsAmount)
    {
        const int32 RandIndex = FMath::RandHelper(SpawnPositions.Num());
        SpawnCube(SpawnPositions[RandIndex]);
        SpawnPositions.RemoveAt(RandIndex);

        if (UKismetMathLibrary::RandomBoolWithWeight(CurrentDifficultyData.ChanceToAddCubeInLine))
        {
            OnSpawnCube();
            return;
        }
    }

    if (bRestartSpawnAfterSpeedChanged)    // Spawn is restarting after speed have been changed.
    {
        GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::OnSpawnCube, GetSpawnTimerRate(), true);
        bRestartSpawnAfterSpeedChanged = false;
    }

    RestoreSpawnPositions();
}

void ACGFieldActor::SpawnCube(int32 SpawnPosition)
{
    const auto RelativeLocation = FVector{CubesSpawnXOffset, SpawnPosition * CubesSpawnStep + CubesSpawnYOffset, CubesSpawnZOffset};
    const auto SpawnTransform = FTransform{RelativeLocation} * GetTransform();
    auto* SpawnedCube = GetWorld() ? GetWorld()->SpawnActorDeferred<ACGCubeActor>(SpawningCubeClass, SpawnTransform) : nullptr;
    check(SpawnedCube);
    const auto CubeType = GetRandomCubeType();
    const auto ColorData = CubeColorDataMap.Contains(CubeType) ? CubeColorDataMap[CubeType] : FCubeColorData{};
    const auto OnSpeedChangedHandle = OnSpeedChanged.AddUObject(SpawnedCube, &ACGCubeActor::OnSpeedChanged);
    SpawnedCube->Init(CurrentDifficultyData.SpeedData, CurrentGameSpeed, CubeType, ColorData, GetSize().X, OnSpeedChangedHandle);
    SpawnedCube->OnEndPlay.BindLambda(
        [&](FDelegateHandle OnSpeedChangedHandle)
        {
            OnSpeedChanged.Remove(OnSpeedChangedHandle);
        });
    SpawnedCube->FinishSpawning(SpawnTransform);
}

void ACGFieldActor::OnGameSpeedChanged(int32 InSpeed)
{
    CurrentGameSpeed = InSpeed;
    bRestartSpawnAfterSpeedChanged = true;
    OnSpeedChanged.Broadcast(CurrentGameSpeed);
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
    if (Multiplier == 1 && !Indicators.IsEmpty())
    {
        for (const auto& Indicator : Indicators)
        {
            Indicator->Teardown();
        }

        Indicators.Empty();
    }
    else
    {
        SpawnIndicator(CubeType, Multiplier);
    }
}

void ACGFieldActor::SpawnIndicator(ECubeType CubeType, int32 Multiplier)
{
    // (Multiplier - 2) because the first an indicator cube spawn when Multiplier be 2.
    const auto RelativeLocation = FVector{(Multiplier - 2) * IndicatorsSpawnStep + IndicatorsSpawnXOffset,    //
                                          SpawnPositionsAmount * CubesSpawnStep + CubesSpawnYOffset,          //
                                          CubesSpawnZOffset};                                                 //
    const auto ColorData = CubeColorDataMap.Contains(CubeType) ? CubeColorDataMap[CubeType] : FCubeColorData{};
    auto* Indicator = SpawnCubeActor<ACGBaseCubeActor>(IndicatorClass, RelativeLocation);
    Indicator->SetColor(ColorData);
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
    if (FieldMaterialColorsMap.Contains(BonusType))
    {
        if (UMaterialInstanceDynamic* DynMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0))
        {
            DynMaterial->SetVectorParameterValue(FieldColorParamName, FieldMaterialColorsMap[BonusType]);
        }
    }
}

void ACGFieldActor::SpawnBonusIndicator(EBonusType BonusType)
{
    const auto RelativeLocation = FVector{BonusIndicatorPosition * IndicatorsSpawnStep + IndicatorsSpawnXOffset,    //
                                          SpawnPositionsAmount * CubesSpawnStep + CubesSpawnYOffset,                //
                                          CubesSpawnZOffset};                                                       //
    const auto ColorData = BonusColorDataMap.Contains(BonusType) ? BonusColorDataMap[BonusType] : FCubeColorData{};
    BonusIndicator = SpawnCubeActor<ACGBaseCubeActor>(IndicatorClass, RelativeLocation);
    BonusIndicator->SetColor(ColorData);
    BonusIndicator->OnDestroyed.AddDynamic(this, &ThisClass::OnBonusIndicatorDestroyed);
}

void ACGFieldActor::OnBonusIndicatorDestroyed(AActor* DestroyedActor)
{
    if (NewBonusIndicatorType == EBonusType::None)
        return;

    SpawnBonusIndicator(NewBonusIndicatorType);
}

void ACGFieldActor::OnAudioPlaybackMagnitude(TArray<float> Magnitudes)
{
    if (Magnitudes.IsEmpty())
        return;

    const int32 SplitFactor = Magnitudes.Num() / (BonusIndicatorPosition + 1);

    for (int32 i = 0; i < Indicators.Num(); ++i)
    {
        if (IsValid(Indicators[i]))
        {
            FVector NewLocation = Indicators[i]->GetActorLocation();
            NewLocation.Z = Magnitudes[i * SplitFactor] * MagnitudeFactor.Z;
            Indicators[i]->SetActorLocation(NewLocation);
        }
    }

    if (IsValid(BonusIndicator))
    {
        FVector NewLocation = BonusIndicator->GetActorLocation();
        NewLocation.Z = Magnitudes[BonusIndicatorPosition * SplitFactor] * MagnitudeFactor.Z;
        BonusIndicator->SetActorLocation(NewLocation);
    }
}
