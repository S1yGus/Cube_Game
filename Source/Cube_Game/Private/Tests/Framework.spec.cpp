// Cube_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "CGUtils.h"
#include "Tests/CGTestUtils.h"
#include "CGGameMode.h"

#define WHAT_FORMAT_STR L"%s: current value: %d, expected value: %d"

using namespace Test;

BEGIN_DEFINE_SPEC(FFramework, "CubeGame", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
ACGGameMode* GameMode;

END_DEFINE_SPEC(FFramework)

void FFramework::Define()
{
    Describe("Framework.GameMode",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("/Game/Tests/TestLevel");
                         World = CGUtils::GetCurrentWorld();
                         TestNotNull("World should exist.", World);
                         GameMode = World->GetAuthGameMode<ACGGameMode>();
                         TestNotNull("GameMode should exist.", GameMode);
                     });

                 It("GameSpeedShouldBeChanged",
                    [this]()
                    {
                        for (int32 i = 0; i < StaticEnum<ECubeType>()->NumEnums() - 2; ++i)
                        {
                            const auto CurrentType = static_cast<ECubeType>(i);
                            const FString CurrentTypeStr = StaticEnum<ECubeType>()->GetDisplayNameTextByIndex(i).ToString();
                            const int32 PrevSpeed = GameMode->GetGameSpeed();
                            const FDifficulty* DifficultyData = GameMode->GetDifficultyData();
                            TestNotNull("DifficultyData should be set.", DifficultyData);

                            GameMode->ChangeGameSpeed(CurrentType);

                            const int32 CurrentValue = GameMode->GetGameSpeed();
                            if (DifficultyData->SpeedChangeMap.Contains(CurrentType))
                            {
                                const int32 ExpectedValue = FMath::Max(1, PrevSpeed + DifficultyData->SpeedChangeMap[static_cast<ECubeType>(i)]);
                                const FString WhatMsg = FString::Printf(WHAT_FORMAT_STR, *CurrentTypeStr, CurrentValue, ExpectedValue);
                                TestTrue(WhatMsg, CurrentValue == ExpectedValue);
                            }
                            else
                            {
                                const FString WhatMsg = FString::Printf(WHAT_FORMAT_STR, *CurrentTypeStr, CurrentValue, PrevSpeed);
                                TestTrue(WhatMsg, CurrentValue == PrevSpeed);
                            }
                        }

                        SpecCloseLevel(World);
                    });

                 It("GameScoreShouldBeChanged",
                    [this]()
                    {
                        for (int32 i = 0; i < StaticEnum<ECubeType>()->NumEnums() - 2; ++i)
                        {
                            const auto CurrentType = static_cast<ECubeType>(i);
                            const FString CurrentTypeStr = StaticEnum<ECubeType>()->GetDisplayNameTextByIndex(i).ToString();
                            const int32 PrevScore = GameMode->GetScore();
                            const FDifficulty* DifficultyData = GameMode->GetDifficultyData();
                            TestNotNull("DifficultyData should be set.", DifficultyData);

                            GameMode->ChangeScore(CurrentType);

                            const int32 CurrentValue = GameMode->GetScore();
                            if (DifficultyData->ScoreChangeMap.Contains(CurrentType))
                            {

                                const int32 ExpectedValue = FMath::Max(0, PrevScore + DifficultyData->ScoreChangeMap[static_cast<ECubeType>(i)]);
                                const FString WhatMsg = FString::Printf(WHAT_FORMAT_STR, *CurrentTypeStr, CurrentValue, ExpectedValue);
                                TestTrue(WhatMsg, CurrentValue == ExpectedValue);
                            }
                            else
                            {
                                const FString WhatMsg = FString::Printf(WHAT_FORMAT_STR, *CurrentTypeStr, CurrentValue, PrevScore);
                                TestTrue(WhatMsg, CurrentValue == PrevScore);
                            }
                        }

                        SpecCloseLevel(World);
                    });

                 It("GameSpeedShouldBeChangedDependingOnTheScore",
                    [this]()
                    {
                        CallFuncByNameWithParams(GameMode, "SetMaxMultiplier", {FString::FromInt(1)});    // Turning off the significance of the multiplier.

                        for (int32 i = 0; i < StaticEnum<ECubeType>()->NumEnums() - 2; ++i)
                        {
                            const auto CurrentType = static_cast<ECubeType>(i);
                            const FString CurrentTypeStr = StaticEnum<ECubeType>()->GetDisplayNameTextByIndex(i).ToString();
                            const int32 PrevSpeed = GameMode->GetGameSpeed();
                            const FDifficulty* DifficultyData = GameMode->GetDifficultyData();
                            TestNotNull("DifficultyData should be set.", DifficultyData);

                            if (DifficultyData->ScoreChangeMap.Contains(CurrentType) && DifficultyData->ScoreChangeMap[CurrentType] >= 0)
                            {
                                const int32 Iterations =
                                    FMath::CeilToInt32(static_cast<float>(DifficultyData->ScoreToSpeedUp) / DifficultyData->ScoreChangeMap[CurrentType]);
                                for (int32 j = 0; j < Iterations; ++j)
                                {
                                    GameMode->ChangeScore(CurrentType);
                                }

                                const int32 CurrentValue = GameMode->GetGameSpeed();
                                const int32 ExpectedValue = PrevSpeed + 1;
                                const FString WhatMsg = FString::Printf(WHAT_FORMAT_STR, *CurrentTypeStr, CurrentValue, ExpectedValue);
                                TestTrue(WhatMsg, CurrentValue == ExpectedValue);
                            }
                            else
                            {
                                GameMode->ChangeScore(CurrentType);

                                const int32 CurrentValue = GameMode->GetGameSpeed();
                                const FString WhatMsg = FString::Printf(WHAT_FORMAT_STR, *CurrentTypeStr, CurrentValue, PrevSpeed);
                                TestTrue(WhatMsg, CurrentValue == PrevSpeed);
                            }
                        }

                        SpecCloseLevel(World);
                    });
             });
}

#endif
