// Cube_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Tests/CGTestUtils.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/WorldSettings.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Settings/CGGameUserSettings.h"
#include "CGGameMode.h"

using namespace Test;

static const FString CubeRef{"Blueprint'/Game/Gameplay/Cubes/BP_CGCubeActor.BP_CGCubeActor'"};

BEGIN_DEFINE_SPEC(FIntegration, "CubeGame", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
ACGGameMode* GameMode;
UCGGameUserSettings* GameUserSettings;

const FChangeData NegativeSet{.Score = -1, .Time = -1, .Speed = 1};
const FChangeData PositiveSet{.Score = 1, .Time = 1, .Speed = -1};

END_DEFINE_SPEC(FIntegration)

void FIntegration::Define()
{
    Describe("Integration.Niagara",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("/Game/Tests/TestLevel");
                         World = GetTestGameWorld();
                         TestNotNull("World should exist.", World);
                         GameMode = World->GetAuthGameMode<ACGGameMode>();
                         TestNotNull("GameMode should exist.", GameMode);
                         GameUserSettings = UCGGameUserSettings::Get();
                         TestNotNull("GameUserSettings should exist.", GameUserSettings);
                     });

                 {
                     using Payload = TArray<TTuple<FString, bool, bool, FString>>;
                     Payload TestPayload{{"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGMissileBonusActor.BP_CGMissileBonusActor'", true, true, "NS_Annihilate"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGMissileBonusActor.BP_CGMissileBonusActor'", false, true, "NS_Annihilate"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGMissileBonusActor.BP_CGMissileBonusActor'", false, false, "NS_Annihilate"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGBombBonusActor.BP_CGBombBonusActor'", true, true, "NS_Annihilate"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGBombBonusActor.BP_CGBombBonusActor'", false, true, "NS_Annihilate"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGBombBonusActor.BP_CGBombBonusActor'", false, false, "NS_Annihilate"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGShieldBonusActor.BP_CGShieldBonusActor'", true, true, "NS_Annihilate"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGShieldBonusActor.BP_CGShieldBonusActor'", false, true, "NS_Annihilate"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGShieldBonusActor.BP_CGShieldBonusActor'", false, false, "NS_Annihilate"}};
                     for (const auto& [BonusRef, bCharged, bCubeNegative, ExpectedAssetName] : TestPayload)
                     {
                         FString BonusName;
                         BonusRef.Split(".BP_CG", nullptr, &BonusName, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                         BonusName.Split("Actor'", &BonusName, nullptr, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                         const FString BonusCondition{bCharged ? "Charged" : "Uncharged"};
                         const FString CubeType{bCubeNegative ? "NegativeCube" : "PositiveCube"};
                         It(FString::Printf(TEXT("AnnihilationEffectShouldBeSpawned.%s%sAnd%s"), *BonusCondition, *BonusName, *CubeType),
                            [=, this]()
                            {
                                const ECubeType CubeType{ECubeType::GoodCube};
                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    //
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    //
                                                          (bCubeNegative ? NegativeSet : PositiveSet).ToString()});

                                const FVector SpawnLocation{300.0};
                                auto* Cube = CreateBlueprint<ACGCubeActor>(World, CubeRef, FTransform{SpawnLocation});
                                if (!TestNotNull("Cube should exist.", Cube))
                                    return;

                                Cube->SetCubeType(CubeType);
                                auto* Bonus = CreateBlueprint<ACGBaseBonusActor>(World, BonusRef, FTransform::Identity);
                                if (!TestNotNull("Bonus should exist.", Bonus))
                                    return;

                                Bonus->SetBonusCharged(bCharged);
                                const auto* PC = World->GetFirstPlayerController();
                                if (!TestNotNull("PlayerController should exist.", PC))
                                    return;

                                Bonus->SetOwner(PC->GetPawn());
                                Bonus->SetActorLocation(SpawnLocation);
                                const auto* WorldSettings = Cast<AWorldSettings>(UGameplayStatics::GetActorOfClass(World, AWorldSettings::StaticClass()));
                                if (!TestNotNull("WorldSettings should exist.", WorldSettings))
                                    return;

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
                     using Payload = TArray<TTuple<FString, bool, FString>>;
                     Payload TestPayload{{"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGMissileBonusActor.BP_CGMissileBonusActor'", false, "NS_Collect"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGBombBonusActor.BP_CGBombBonusActor'", false, "NS_Collect"},
                                         {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGShieldBonusActor.BP_CGShieldBonusActor'", false, "NS_Collect"}};
                     for (const auto& [BonusRef, bCubeNegative, ExpectedAssetName] : TestPayload)
                     {
                         FString BonusName;
                         BonusRef.Split(".BP_CG", nullptr, &BonusName, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                         BonusName.Split("Actor'", &BonusName, nullptr, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                         const FString CubeName{bCubeNegative ? "NegativeCube" : "PositiveCube"};
                         It(FString::Printf(TEXT("CollectEffectShouldBeSpawned.%sAnd%s"), *BonusName, *CubeName),
                            [=, this]()
                            {
                                const ECubeType CubeType{ECubeType::GoodCube};
                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    //
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    //
                                                          (bCubeNegative ? NegativeSet : PositiveSet).ToString()});

                                const FVector SpawnLocation{300.0};
                                auto* Cube = CreateBlueprint<ACGCubeActor>(World, CubeRef, FTransform{SpawnLocation});
                                if (!TestNotNull("Cube should exist.", Cube))
                                    return;

                                Cube->SetCubeType(CubeType);
                                auto* Bonus = CreateBlueprint<ACGBaseBonusActor>(World, BonusRef, FTransform::Identity);
                                if (!TestNotNull("Bonus should exist.", Bonus))
                                    return;

                                Bonus->SetBonusCharged(true);
                                const auto* PC = World->GetFirstPlayerController();
                                if (!TestNotNull("PlayerController should exist.", PC))
                                    return;

                                auto* Pawn = PC->GetPawn();
                                if (!TestNotNull("Pawn should exist.", Pawn))
                                    return;

                                Bonus->SetOwner(Pawn);
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

                 AfterEach(
                     [this]()
                     {
                         SpecCloseLevel(World);
                     });
             });
}

#endif
