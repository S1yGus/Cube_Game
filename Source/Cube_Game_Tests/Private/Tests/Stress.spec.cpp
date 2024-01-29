// Cube_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Player/Components/CGBonusComponent.h"

DEFINE_SPEC(FStress, "CubeGame", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority)

void FStress::Define()
{
    Describe("Components.BonusComponent",
             [this]()
             {
                 It("ChargedBonusShouldBeChangedToDifferentType",
                    [this]()
                    {
                        auto* BonusComponent = NewObject<UCGBonusComponent>();

                        bool bLastChargeCondition{false};
                        bool bChargeCondition{false};
                        BonusComponent->OnBonusCharged.AddLambda(
                            [&](bool bInCharged)
                            {
                                bChargeCondition = bInCharged;
                            });
                        EBonusType LastBonusType{EBonusType::None};
                        EBonusType BonusType{EBonusType::None};
                        BonusComponent->OnBonusChanged.AddLambda(
                            [&](EBonusType InBonusType)
                            {
                                BonusType = InBonusType;
                            });

                        for (int32 i = 0; i < 1000000; ++i)
                        {
                            BonusComponent->CollectBonusCube();

                            if (i % 2 == 0)
                            {
                                TestTrueExpr(LastBonusType != BonusType);
                            }
                            else
                            {
                                TestTrueExpr(bChargeCondition && !bLastChargeCondition);
                            }

                            bLastChargeCondition = bChargeCondition;
                            LastBonusType = BonusType;
                        }
                    });
             });
}

#endif
