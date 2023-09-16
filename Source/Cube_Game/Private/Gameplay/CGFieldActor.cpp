// Cube_Game, All rights reserved.

#include "Gameplay/CGFieldActor.h"
#include "CGGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Cubes/CGBaseCubeActor.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "Player/Components/CGBonusComponent.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/Components/CGMusicComponent.h"

constexpr static float IndicatorAmplitudeMultiplier{200.0f};

ACGFieldActor::ACGFieldActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    check(StaticMeshComponent);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->CastShadow = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
    check(WidgetComponent);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawAtDesiredSize(true);
    WidgetComponent->SetupAttachment(StaticMeshComponent);

    MusicComponent = CreateDefaultSubobject<UCGMusicComponent>("MusicComponent");
    check(MusicComponent);

    SetRootComponent(StaticMeshComponent);
}

void ACGFieldActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    MusicComponent->OnSubmixSpectralAnalysis.BindDynamic(this, &ThisClass::OnSpectralAnalysis);
}

void ACGFieldActor::BeginPlay()
{
    Super::BeginPlay();

    if (auto* GameMode = GetGameMode())
    {
        GameMode->OnSpeedChanged.AddUObject(this, &ThisClass::OnSpeedChanged);
        GameMode->OnMultiplierChanged.AddUObject(this, &ThisClass::OnMultiplierChanged);
        BonusIndicatorPosition = GameMode->GetMaxMultiplier() - 1;
    }

    if (const auto* PlayerPawn = GetPlayerPawn())
    {
        if (auto* BonusComponent = PlayerPawn->FindComponentByClass<UCGBonusComponent>())
        {
            BonusComponent->OnBonusChanged.AddUObject(this, &ThisClass::OnBonusChanged);
        }
    }

    ChangeFieldColor(EBonusType::None);    // Set default field color.

    RestoreSpawnPositions();
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::OnSpawnCube, GetSpawnTimerRate(), true);
}

float ACGFieldActor::GetSpawnTimerRate() const
{
    const auto* GameMode = GetGameMode();
    const auto* DifficultyData = GetDifficultyData();
    if (GameMode && DifficultyData)
    {
        return DifficultyData->DistanceBetweenCubes / GameMode->GetCubeSpeed();
    }

    return 0.0f;
}

ECubeType ACGFieldActor::GetRandomCubeType() const
{
    if (const FDifficulty* DifficultyData = GetDifficultyData())
    {
        const float RandNum = FMath::FRand();
        TArray<ECubeType> RandItems;
        for (const auto& [Type, Probability] : DifficultyData->SpawnWeightMap)
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
    }

    return ECubeType::None;
}

ACGGameMode* ACGFieldActor::GetGameMode() const
{
    return GetWorld() ? GetWorld()->GetAuthGameMode<ACGGameMode>() : nullptr;
}

const FDifficulty* ACGFieldActor::GetDifficultyData() const
{
    if (const auto* GameMode = GetGameMode(); GameMode && GameMode->GetDifficultyData())
    {
        return GameMode->GetDifficultyData();
    }

    return nullptr;
}

const APawn* ACGFieldActor::GetPlayerPawn() const
{
    if (GetWorld())
    {
        if (const auto* PC = GetWorld()->GetFirstPlayerController())
        {
            return PC->GetPawn();
        }
    }

    return nullptr;
}

void ACGFieldActor::OnSpawnCube()
{
    if (const FDifficulty* DifficultyData = GetDifficultyData())
    {
        if (++CubesInLine <= DifficultyData->MaxNumOfCubesInLine && CubesInLine <= SpawnPositionsAmount)
        {
            const int32 RandIndex = FMath::RandHelper(SpawnPositions.Num());
            SpawnCube(SpawnPositions[RandIndex]);
            SpawnPositions.RemoveAt(RandIndex);

            if (UKismetMathLibrary::RandomBoolWithWeight(DifficultyData->ChanceToAddCubeInLine))
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
}

void ACGFieldActor::SpawnCube(int32 SpawnPosition)
{
    const FVector RelativeLocation{SpawnPosition * SpawnStep + SpawnXOffset, SpawnYOffset, SpawnZOffset};
    const ECubeType CubeType = GetRandomCubeType();
    const auto CubeColorData = CubeColorDataMap.Contains(CubeType) ? CubeColorDataMap[CubeType] : FCubeColorData{};
    auto* SpawnedCube = SpawnCubeActor<ACGCubeActor>(SpawningCubeClass, RelativeLocation, CubeColorData);
    SpawnedCube->SetCubeType(CubeType);
}

void ACGFieldActor::OnSpeedChanged(int32 NewSpeed)
{
    bRestartSpawnAfterSpeedChanged = true;
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
    const FVector RelativeLocation{SpawnPositionsAmount * SpawnStep + SpawnXOffset,                     //
                                   (Multiplier - 2) * -IndicatorsSpawnStep + IndicatorsSpawnYOffset,    //
                                   IndicatorsSpawnZOffset};                                             //
    const auto CubeColorData = CubeColorDataMap.Contains(CubeType) ? CubeColorDataMap[CubeType] : FCubeColorData{};
    auto* Indicator = SpawnCubeActor<ACGBaseCubeActor>(IndicatorClass, RelativeLocation, CubeColorData);
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

void ACGFieldActor::OnSpectralAnalysis(const TArray<float>& Magnitudes)
{
    for (int32 i = 0; i < Indicators.Num(); ++i)
    {
        if (IsValid(Indicators[i]) && i < Magnitudes.Num())
        {
            FVector NewLocation = Indicators[i]->GetActorLocation();
            NewLocation.Z = Magnitudes[i] * IndicatorAmplitudeMultiplier;
            Indicators[i]->SetActorLocation(NewLocation);
        }
    }

    if (IsValid(BonusIndicator) && BonusIndicatorPosition < Magnitudes.Num())
    {
        FVector NewLocation = BonusIndicator->GetActorLocation();
        NewLocation.Z = Magnitudes[BonusIndicatorPosition] * IndicatorAmplitudeMultiplier;
        BonusIndicator->SetActorLocation(NewLocation);
    }
}
