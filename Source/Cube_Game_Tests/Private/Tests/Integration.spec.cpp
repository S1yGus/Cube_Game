// Cube_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Tests/CGTestUtils.h"
#include "World/Cubes/CGCubeActor.h"
#include "World/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/WorldSettings.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Settings/CGGameUserSettings.h"
#include "CGGameMode.h"
#include "Player/Components/CGBonusComponent.h"

using namespace Test;

BEGIN_DEFINE_SPEC(FIntegration, "CubeGame", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
ACGGameMode* GameMode;
UCGGameUserSettings* GameUserSettings;
APawn* Pawn;
UCGBonusComponent* BonusComponent;
ACGCubeActor* Cube;

const FString CubeRef{"Blueprint'/Game/World/Cubes/BP_CubeActor.BP_CubeActor'"};
const FString MissileBonusRef{"Blueprint'/Game/World/Cubes/Bonuses/BP_MissileBonusActor.BP_MissileBonusActor'"};
const FString BombBonusRef{"Blueprint'/Game/World/Cubes/Bonuses/BP_BombBonusActor.BP_BombBonusActor'"};
const FString ShieldBonusRef{"Blueprint'/Game/World/Cubes/Bonuses/BP_ShieldBonusActor.BP_ShieldBonusActor'"};
const FVector SpawnLocation{500.0};
const ECubeType CubeType{ECubeType::GoodCube};
const FChangeData NegativeSet{-1, -1, 1};
const FChangeData PositiveSet{1, 1, -1};

END_DEFINE_SPEC(FIntegration)

void FIntegration::Define()
{
    Describe("Integration",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("/Game/Tests/TestLevel");

                         World = GetTestGameWorld();
                         if (!TestNotNull("World should exist.", World))
                             return;

                         Pawn = World->GetFirstPlayerController() ? World->GetFirstPlayerController()->GetPawn() : nullptr;
                         if (!TestNotNull("Pawn should exist.", Pawn))
                             return;

                         BonusComponent = Pawn->FindComponentByClass<UCGBonusComponent>();
                         if (!TestNotNull("BonusComponent should exist.", BonusComponent))
                             return;

                         Cube = CreateBlueprintDeferred<ACGCubeActor>(World, CubeRef, FTransform{SpawnLocation});
                         if (!TestNotNull("Cube should exist.", Cube))
                             return;

                         Cube->Init({1, 0}, 0, CubeType, {}, 0, {});
                         Cube->FinishSpawning(FTransform{SpawnLocation});
                         GameMode = World->GetAuthGameMode<ACGGameMode>();
                         if (!TestNotNull("GameMode should exist.", GameMode))
                             return;

                         GameUserSettings = UCGGameUserSettings::Get();
                         TestNotNull("GameUserSettings should exist.", GameUserSettings);
                     });

                 {
                     const FString AnnihilateEffectName{"NS_Annihilate"};
                     using Payload = TArray<TTuple<FString, bool, bool, FString>>;
                     const Payload TestPayload{{MissileBonusRef, true, true, AnnihilateEffectName},      //
                                               {MissileBonusRef, false, true, AnnihilateEffectName},     //
                                               {MissileBonusRef, false, false, AnnihilateEffectName},    //
                                               {BombBonusRef, true, true, AnnihilateEffectName},         //
                                               {BombBonusRef, false, true, AnnihilateEffectName},        //
                                               {BombBonusRef, false, false, AnnihilateEffectName},       //
                                               {ShieldBonusRef, true, true, AnnihilateEffectName},       //
                                               {ShieldBonusRef, false, true, AnnihilateEffectName},      //
                                               {ShieldBonusRef, false, false, AnnihilateEffectName}};
                     for (const auto& [BonusRef, bCharged, bCubeNegative, ExpectedAssetName] : TestPayload)
                     {
                         FString BonusName;
                         BonusRef.Split(".BP_", nullptr, &BonusName, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                         BonusName.Split("Actor'", &BonusName, nullptr, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                         const FString BonusCondition{bCharged ? "Charged" : "Uncharged"};
                         const FString CubeEffect{bCubeNegative ? "Negative" : "Positive"};
                         It(FString::Printf(TEXT("Niagara."
                                                 "AnnihilationEffectShouldBeSpawned."
                                                 "%s%sAnd%sCube"),
                                            *BonusCondition, *BonusName, *CubeEffect),
                            [=, this]()
                            {
                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                          (bCubeNegative ? NegativeSet : PositiveSet).ToString(),                            // ChangeData
                                                          FString::FromInt(1)});                                                             // ScoreToSpeedUp

                                auto* Bonus = CreateBlueprint<ACGBaseBonusActor>(World, BonusRef, FTransform::Identity);
                                if (!TestNotNull("Bonus should exist.", Bonus))
                                    return;

                                Bonus->SetBonusCharged(bCharged);
                                Bonus->OnBonusBeginOverlap.AddLambda(
                                    [&](ACGCubeActor* Cube, bool bCharged)
                                    {
                                        BonusComponent->OnBonusBeginOverlap.Broadcast(Cube, bCharged);
                                    });
                                const auto* WorldSettings = Cast<AWorldSettings>(UGameplayStatics::GetActorOfClass(World, AWorldSettings::StaticClass()));
                                if (!TestNotNull("WorldSettings should exist.", WorldSettings))
                                    return;

                                Bonus->SetActorLocation(SpawnLocation);

                                const auto* Found = Algo::FindByPredicate(WorldSettings->GetComponents(),
                                                                          [&](const auto* Component)
                                                                          {
                                                                              if (const auto* NiagaraComponent = Cast<UNiagaraComponent>(Component);    //
                                                                                  NiagaraComponent && NiagaraComponent->GetAsset())
                                                                              {
                                                                                  return NiagaraComponent->GetAsset()->GetName().Equals(ExpectedAssetName);
                                                                              }
                                                                              return false;
                                                                          });
                                TestTrueExpr(Found != nullptr);
                            });
                     }
                 }

                 {
                     const FString CollectEffectName{"NS_Collect"};
                     using Payload = TArray<TTuple<FString, bool, FString>>;
                     const Payload TestPayload{{MissileBonusRef, false, CollectEffectName},    //
                                               {BombBonusRef, false, CollectEffectName},       //
                                               {ShieldBonusRef, false, CollectEffectName}};
                     for (const auto& [BonusRef, bCubeNegative, ExpectedAssetName] : TestPayload)
                     {
                         FString BonusName;
                         BonusRef.Split(".BP_", nullptr, &BonusName, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                         BonusName.Split("Actor'", &BonusName, nullptr, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                         const FString CubeName{bCubeNegative ? "NegativeCube" : "PositiveCube"};
                         It(FString::Printf(TEXT("Niagara."
                                                 "CollectEffectShouldBeSpawned."
                                                 "%sAnd%s"),
                                            *BonusName, *CubeName),
                            [=, this]()
                            {
                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                          (bCubeNegative ? NegativeSet : PositiveSet).ToString(),                            // ChangeData
                                                          FString::FromInt(1)});                                                             // ScoreToSpeedUp

                                auto* Bonus = CreateBlueprint<ACGBaseBonusActor>(World, BonusRef, FTransform::Identity);
                                if (!TestNotNull("Bonus should exist.", Bonus))
                                    return;

                                Bonus->SetBonusCharged(true);
                                Bonus->OnBonusBeginOverlap.AddLambda(
                                    [&](ACGCubeActor* Cube, bool bCharged)
                                    {
                                        BonusComponent->OnBonusBeginOverlap.Broadcast(Cube, bCharged);
                                    });

                                Bonus->SetActorLocation(SpawnLocation);

                                const auto* Found = Algo::FindByPredicate(Pawn->GetComponents(),
                                                                          [&](const auto* Component)
                                                                          {
                                                                              if (const auto* NiagaraComponent = Cast<UNiagaraComponent>(Component);    //
                                                                                  NiagaraComponent && NiagaraComponent->GetAsset())
                                                                              {
                                                                                  return NiagaraComponent->GetAsset()->GetName().Equals(ExpectedAssetName);
                                                                              }
                                                                              return false;
                                                                          });
                                TestTrueExpr(Found != nullptr);
                            });
                     }
                 }

                 {
                     using Payload = TArray<FChangeData>;
                     const Payload TestPayload{{0, 0, 0}, {42, 0, 0}, {0, 42, 0}, {0, 0, 42}, {42, 42, 42}, {42, 42, 0}, {42, 0, 42}, {0, 42, 42}};
                     for (const auto& Payload : TestPayload)
                     {
                         It(FString::Printf(TEXT("AfterPlayerPicksCubeGameParametersShouldChange."
                                                 "Score(%d)Time(%d)Speed(%d)"),
                                            Payload.Score, Payload.Time, Payload.Speed),
                            [=, this]()
                            {
                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                          Payload.ToString(),                                                                // ChangeData
                                                          FString::FromInt(Payload.Score + 1)});                                             // ScoreToSpeedUp
                                const auto ScoreBefore{GameMode->GetScore()};
                                const auto TimeBefore{GameMode->GetGameTime()};
                                const auto SpeedBefore{GameMode->GetGameSpeed()};

                                Cube->SetActorLocation(Pawn->GetActorLocation());

                                TestTrueExpr(GameMode->GetScore() == ScoreBefore + Payload.Score);
                                TestTrueExpr(GameMode->GetGameTime() == TimeBefore + Payload.Time);
                                TestTrueExpr(GameMode->GetGameSpeed() == SpeedBefore + Payload.Speed);
                            });
                     }
                 }

                 {
                     using Payload = TArray<TTuple<FString, FChangeData>>;
                     const Payload TestPayload{{MissileBonusRef, FChangeData{0, 0, 0}},    //
                                               {BombBonusRef, FChangeData{42, 0, 0}},      //
                                               {ShieldBonusRef, FChangeData{0, 42, 0}},    //
                                               {MissileBonusRef, FChangeData{42, 42, 0}}};
                     for (const auto& [BonusRef, ChangeData] : TestPayload)
                     {
                         It(FString::Printf(TEXT("AfterBonusPicksCubeGameParametersShouldChange."
                                                 "Score(%d)Time(%d)Speed(%d)"),
                                            ChangeData.Score, ChangeData.Time, ChangeData.Speed),
                            [=, this]()
                            {
                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                          ChangeData.ToString(),                                                             // ChangeData
                                                          FString::FromInt(ChangeData.Score + 1)});                                          // ScoreToSpeedUp
                                auto* Bonus = CreateBlueprint<ACGBaseBonusActor>(World, BonusRef, FTransform::Identity);
                                if (!TestNotNull("Bonus should exist.", Bonus))
                                    return;

                                Bonus->SetBonusCharged(true);
                                Bonus->OnBonusBeginOverlap.AddLambda(
                                    [&](ACGCubeActor* Cube, bool bCharged)
                                    {
                                        BonusComponent->OnBonusBeginOverlap.Broadcast(Cube, bCharged);
                                    });
                                const auto ScoreBefore{GameMode->GetScore()};
                                const auto TimeBefore{GameMode->GetGameTime()};
                                const auto SpeedBefore{GameMode->GetGameSpeed()};

                                Bonus->SetActorLocation(SpawnLocation);

                                TestTrueExpr(GameMode->GetScore() == ScoreBefore + ChangeData.Score);
                                TestTrueExpr(GameMode->GetGameTime() == TimeBefore + ChangeData.Time);
                                TestTrueExpr(GameMode->GetGameSpeed() == SpeedBefore + ChangeData.Speed);
                            });
                     }
                 }

                 AfterEach(
                     [this]()
                     {
                         SpecCloseLevel(World);
                     });
             });
}

#endif
