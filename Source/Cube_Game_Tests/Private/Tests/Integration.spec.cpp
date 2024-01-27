// Cube_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Gameplay/Cubes/CGCubeActor.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/WorldSettings.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

using namespace Test;

static const FString CubeRef{"Blueprint'/Game/Gameplay/Cubes/BP_CGCubeActor.BP_CGCubeActor'"};

BEGIN_DEFINE_SPEC(FIntegration, "CubeGame", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;

END_DEFINE_SPEC(FIntegration)

void FIntegration::Define()
{
    Describe("Integration.Niagara.AnnihilationEffectShouldBeSpawned",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("/Game/Tests/TestLevel");
                         World = GetTestGameWorld();
                         TestNotNull("World should exist.", World);
                     });

                 using Payload = TArray<TTuple<FString, bool, ECubeType, FString>>;
                 Payload TestPayload{
                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGMissileBonusActor.BP_CGMissileBonusActor'", true, ECubeType::BadCube, "NS_Annihilate"},
                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGMissileBonusActor.BP_CGMissileBonusActor'", false, ECubeType::BadCube, "NS_Annihilate"},
                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGMissileBonusActor.BP_CGMissileBonusActor'", false, ECubeType::GoodCube, "NS_Annihilate"},
                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGBombBonusActor.BP_CGBombBonusActor'", true, ECubeType::BadCube, "NS_Annihilate"},
                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGBombBonusActor.BP_CGBombBonusActor'", false, ECubeType::BadCube, "NS_Annihilate"},
                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGBombBonusActor.BP_CGBombBonusActor'", false, ECubeType::GoodCube, "NS_Annihilate"},
                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGShieldBonusActor.BP_CGShieldBonusActor'", true, ECubeType::BadCube, "NS_Annihilate"},
                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGShieldBonusActor.BP_CGShieldBonusActor'", false, ECubeType::BadCube, "NS_Annihilate"},
                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGShieldBonusActor.BP_CGShieldBonusActor'", false, ECubeType::GoodCube, "NS_Annihilate"}};
                 for (auto& [BonusRef, bCharged, CubeType, ExpectedAssetName] : TestPayload)
                 {
                     FString BonusName;
                     BonusRef.Split(".BP_CG", nullptr, &BonusName, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                     BonusName.Split("Actor'", &BonusName, nullptr, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                     const FString BonusCondition{bCharged ? "Charged" : "Uncharged"};
                     const FString CubeName{StaticEnum<ECubeType>()->GetNameStringByValue(static_cast<int64>(CubeType))};
                     It(FString::Printf(TEXT("%s%sAnd%s"), *BonusCondition, *BonusName, *CubeName),
                        [=, this]()
                        {
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

                 AfterEach(
                     [this]()
                     {
                         SpecCloseLevel(World);
                     });
             });

    Describe("Integration.Niagara.CollectEffectShouldBeSpawned",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("/Game/Tests/TestLevel");
                         World = GetTestGameWorld();
                         TestNotNull("World should exist.", World);
                     });

                 using Payload = TArray<TTuple<FString, ECubeType, FString>>;
                 Payload TestPayload{{"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGMissileBonusActor.BP_CGMissileBonusActor'", ECubeType::GoodCube, "NS_Collect"},
                                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGBombBonusActor.BP_CGBombBonusActor'", ECubeType::GoodCube, "NS_Collect"},
                                     {"Blueprint'/Game/Gameplay/Cubes/Bonuses/BP_CGShieldBonusActor.BP_CGShieldBonusActor'", ECubeType::GoodCube, "NS_Collect"}};
                 for (auto& [BonusRef, CubeType, ExpectedAssetName] : TestPayload)
                 {
                     FString BonusName;
                     BonusRef.Split(".BP_CG", nullptr, &BonusName, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                     BonusName.Split("Actor'", &BonusName, nullptr, ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
                     const FString CubeName{StaticEnum<ECubeType>()->GetNameStringByValue(static_cast<int64>(CubeType))};
                     It(FString::Printf(TEXT("%sAnd%s"), *BonusName, *CubeName),
                        [=, this]()
                        {
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

                 AfterEach(
                     [this]()
                     {
                         SpecCloseLevel(World);
                     });
             });
}

#endif
