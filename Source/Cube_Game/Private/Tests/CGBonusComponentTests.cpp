// Cube_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/CGBonusComponentTests.h"
#include "CoreMinimal.h"
#include "CGCoreTypes.h"
#include "CGUtils.h"
#include "Tests/CGTestUtils.h"
#include "Misc/AutomationTest.h"
#include "Player/Components/CGBonusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Cubes/Bonuses/CGBaseBonusActor.h"
#include "Player/CGPlayer.h"

using namespace Test;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBonusComponentTests, "CubeGame.Components.BonusComponentTests",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBonusComponentTests::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Maps/Test");

    {
        const auto World = CGUtils::GetCurrentWorld();
        const FString BlueprintName{"Blueprint'/Game/Player/BP_CGPlayer.BP_CGPlayer'"};
        const FTransform SpawnTransform{FVector{1000.0f}};
        const auto Playr = SpawnBlueprint<ACGPlayer>(World, BlueprintName, SpawnTransform);
        if (!TestNotNull("Player must exists.", Playr))
            return false;

        const auto BonusComponent = Playr->FindComponentByClass<UCGBonusComponent>();
        if (!TestNotNull("BonusComponent must exists.", BonusComponent))
            return false;

        AddInfo("BonusComponent tick should be disabled.");

        TestFalse("Tick should be disabled.", BonusComponent->IsComponentTickEnabled());

        AddInfo("Component shouldn't spawn bonus by default.");

        BonusComponent->UseCurrentBonus();
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(World, ACGBaseBonusActor::StaticClass(), FoundActors);
        TestTrueExpr(FoundActors.Num() == 0);

        AddInfo("Bonus should be spawned after run SetRandomBonus().");

        BonusComponent->SetRandomBonus();
        BonusComponent->UseCurrentBonus();
        UGameplayStatics::GetAllActorsOfClass(World, ACGBaseBonusActor::StaticClass(), FoundActors);
        TestTrueExpr(FoundActors.Num() == 1);
    }

    {
        auto BonusComponent = NewObject<UCGBonusComponent>();
        if (!TestNotNull("BonusComponent must exists.", BonusComponent))
            return false;

        AddInfo("Bonus should be changed after run SetRandomBonus().");

        bool bBonusNotNone = false;
        BonusComponent->OnBonusChanged.AddLambda(
            [&bBonusNotNone](EBonusType NewBonusType) -> void
            {
                if (NewBonusType != EBonusType::None)
                {
                    bBonusNotNone = true;
                }
            });
        BonusComponent->SetRandomBonus();
        TestTrueExpr(bBonusNotNone);

        AddInfo("Bonus should be charged after run SetRandomBonus() the second time.");

        bool bBonusCharged = false;
        BonusComponent->OnBonusCharged.AddLambda(
            [&bBonusCharged](bool bCharged) -> void
            {
                bBonusCharged = bCharged;
            });
        BonusComponent->SetRandomBonus();
        TestTrueExpr(bBonusCharged);
        TestTrueExpr(bBonusNotNone);

        AddInfo("Bonus should be uncharged after run SetRandomBonus() the third time.");

        BonusComponent->SetRandomBonus();
        TestTrueExpr(!bBonusCharged);
        TestTrueExpr(bBonusNotNone);
    }

    return true;
}

#endif