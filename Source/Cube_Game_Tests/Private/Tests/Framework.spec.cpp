// Cube_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/TestUtils.h"
#include "Tests/CGTestUtils.h"
#include "CGGameMode.h"
#include "Settings/CGGameUserSettings.h"
#include "Player/CGPlayer.h"
#include "Player/Components/CGBonusComponent.h"
#include "Slate/SceneViewport.h"

#define WHAT_FORMAT_STR L"%s: current value: %d, expected value: %d"

using namespace Test;

BEGIN_DEFINE_SPEC(FFramework, "CubeGame", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
ACGGameMode* GameMode;
UCGGameUserSettings* GameUserSettings;

END_DEFINE_SPEC(FFramework)

void FFramework::Define()
{
    Describe("Framework.GameMode",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("/Game/Levels/GameLevel");
                         World = GetTestGameWorld();
                         TestNotNull("World should exist.", World);
                         GameMode = World->GetAuthGameMode<ACGGameMode>();
                         TestNotNull("GameMode should exist.", GameMode);
                         GameUserSettings = UCGGameUserSettings::Get();
                         TestNotNull("GameUserSettings should exist.", GameUserSettings);
                     });

                 It("AllDifficultyDataShouldBeSet",
                    [this]()
                    {
                        const UEnum* DifficultyEnum = StaticEnum<EDifficulty>();
                        for (int32 i = 0; i < DifficultyEnum->NumEnums() - 2; ++i)
                        {
                            GameUserSettings->SetDifficulty(static_cast<EDifficulty>(i));
                            TestTrueExpr(GameMode->GetDifficultyData() != nullptr);
                        }
                    });

                 AfterEach(
                     [this]()
                     {
                         SpecCloseLevel(World);
                     });
             });

    Describe("Framework.GameMode",
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
                    });

                 It("UnshownHintsShouldBeQueued", EAsyncExecution::ThreadPool,
                    [this]()
                    {
                        const APawn* PlayerPawn = World->GetFirstPlayerController() ? World->GetFirstPlayerController()->GetPawn() : nullptr;
                        if (!TestNotNull("PlayerPawn should exist.", PlayerPawn))
                            return;

                        const auto* BonusComponent = PlayerPawn->FindComponentByClass<UCGBonusComponent>();
                        if (!TestNotNull("BonusComponent should exist.", BonusComponent))
                            return;

                        int32 PopUpHintAmount{0};
                        const FHintsStatus OldHintsStatus = GameUserSettings->GetHintsStatus();
                        const FHintsStatus NewHintsStatus = {{EHintType::Startup, false},   {EHintType::Multiplier, false},   {EHintType::LowTime, false},
                                                             {EHintType::SpeedUp, false},   {EHintType::BonusCharged, false}, {EHintType::GoodCube, false},
                                                             {EHintType::BadCube, false},   {EHintType::ScoreCube, false},    {EHintType::TimeCube, false},
                                                             {EHintType::BonusCube, false}, {EHintType::SpeedCube, false},    {EHintType::VeryBadCube, false}};
                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameMode->OnShowPopUpHint.AddLambda(
                                          [&](const FHintData&)
                                          {
                                              ++PopUpHintAmount;
                                          });

                                      GameUserSettings->OnHintsStatusChanged.Broadcast(NewHintsStatus);

                                      for (int32 i = 0; i < StaticEnum<ECubeType>()->NumEnums() - 3; ++i)
                                      {
                                          GameMode->EnqueueHint(static_cast<ECubeType>(i));
                                      }
                                      GameMode->OnMultiplierChanged.Broadcast(static_cast<ECubeType>(0), 0);
                                      GameMode->OnLowTime.Broadcast();
                                      GameMode->OnSpeedChanged.Broadcast(0);
                                      BonusComponent->OnBonusCharged.Broadcast(true);
                                  });

                        const float NextHintDelay = GetPropertyValueByName<float>(GameMode, "NextHintDelay");
                        const float StartupHintDelay = GetPropertyValueByName<float>(GameMode, "StartupHintDelay");
                        const float ThreadSyncDelta{0.5f};
                        FPlatformProcess::Sleep(FMath::Max(NextHintDelay * NewHintsStatus.Num(), StartupHintDelay) + ThreadSyncDelta);

                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameUserSettings->SetHintsStatus(OldHintsStatus);
                                  });
                        FPlatformProcess::Sleep(ThreadSyncDelta);

                        TestTrueExpr(PopUpHintAmount == NewHintsStatus.Num());
                    });

                 It("ShownHintsShouldNotBeQueued", EAsyncExecution::ThreadPool,
                    [this]()
                    {
                        const APawn* PlayerPawn = World->GetFirstPlayerController() ? World->GetFirstPlayerController()->GetPawn() : nullptr;
                        if (!TestNotNull("PlayerPawn should exist.", PlayerPawn))
                            return;

                        const auto* BonusComponent = PlayerPawn->FindComponentByClass<UCGBonusComponent>();
                        if (!TestNotNull("BonusComponent should exist.", BonusComponent))
                            return;

                        int32 PopUpHintAmount{0};
                        const FHintsStatus OldHintsStatus = GameUserSettings->GetHintsStatus();
                        const FHintsStatus NewHintsStatus = {{EHintType::Startup, true},   {EHintType::Multiplier, true},   {EHintType::LowTime, true},
                                                             {EHintType::SpeedUp, true},   {EHintType::BonusCharged, true}, {EHintType::GoodCube, true},
                                                             {EHintType::BadCube, true},   {EHintType::ScoreCube, true},    {EHintType::TimeCube, true},
                                                             {EHintType::BonusCube, true}, {EHintType::SpeedCube, true},    {EHintType::VeryBadCube, true}};
                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameMode->OnShowPopUpHint.AddLambda(
                                          [&](const FHintData&)
                                          {
                                              ++PopUpHintAmount;
                                          });

                                      GameUserSettings->OnHintsStatusChanged.Broadcast(NewHintsStatus);

                                      for (int32 i = 1; i < StaticEnum<ECubeType>()->NumEnums() - 2; ++i)
                                      {
                                          GameMode->EnqueueHint(static_cast<ECubeType>(i));
                                      }
                                      GameMode->OnMultiplierChanged.Broadcast(static_cast<ECubeType>(0), 0);
                                      GameMode->OnLowTime.Broadcast();
                                      GameMode->OnSpeedChanged.Broadcast(0);
                                      BonusComponent->OnBonusCharged.Broadcast(true);
                                  });

                        const float NextHintDelay = GetPropertyValueByName<float>(GameMode, "NextHintDelay");
                        const float StartupHintDelay = GetPropertyValueByName<float>(GameMode, "StartupHintDelay");
                        const float ThreadSyncDelta{0.5f};
                        FPlatformProcess::Sleep(FMath::Max(NextHintDelay * NewHintsStatus.Num(), StartupHintDelay) + ThreadSyncDelta);

                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameUserSettings->SetHintsStatus(OldHintsStatus);
                                  });
                        FPlatformProcess::Sleep(ThreadSyncDelta);

                        TestTrueExpr(PopUpHintAmount == 0);
                    });

                 It("HintsShouldBeShownOnlyOnce", EAsyncExecution::ThreadPool,
                    [this]()
                    {
                        int32 PopUpHintAmount{0};
                        const FHintsStatus OldHintsStatus = GameUserSettings->GetHintsStatus();
                        const FHintsStatus NewHintsStatus = {{EHintType::Startup, true},   {EHintType::Multiplier, true},   {EHintType::LowTime, true},
                                                             {EHintType::SpeedUp, true},   {EHintType::BonusCharged, true}, {EHintType::GoodCube, false},
                                                             {EHintType::BadCube, true},   {EHintType::ScoreCube, true},    {EHintType::TimeCube, true},
                                                             {EHintType::BonusCube, true}, {EHintType::SpeedCube, true},    {EHintType::VeryBadCube, true}};
                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameMode->OnShowPopUpHint.AddLambda(
                                          [&](const FHintData&)
                                          {
                                              ++PopUpHintAmount;
                                          });

                                      GameUserSettings->OnHintsStatusChanged.Broadcast(NewHintsStatus);

                                      GameMode->EnqueueHint(ECubeType::GoodCube);
                                      GameMode->EnqueueHint(ECubeType::GoodCube);
                                      GameMode->EnqueueHint(ECubeType::GoodCube);
                                  });

                        const float NextHintDelay = GetPropertyValueByName<float>(GameMode, "NextHintDelay");
                        const float ThreadSyncDelta{0.5f};
                        FPlatformProcess::Sleep(NextHintDelay * 3 + ThreadSyncDelta);

                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameUserSettings->SetHintsStatus(OldHintsStatus);
                                  });
                        FPlatformProcess::Sleep(ThreadSyncDelta);

                        TestTrueExpr(PopUpHintAmount == 1);
                    });

                 {
                     using Payload = TArray<TTuple<ECubeType, FChangeData, bool>>;
                     // clang-format off
                     Payload TestPayload{{ECubeType::BadCube,     FChangeData{ 1,  1,  1}, true},
                                         {ECubeType::BonusCube,   FChangeData{ 1,  1, -1}, false},
                                         {ECubeType::GoodCube,    FChangeData{ 1,  1,  0}, false},
                                         {ECubeType::ScoreCube,   FChangeData{ 1, -1,  1}, true},
                                         {ECubeType::SpeedCube,   FChangeData{ 1, -1, -1}, true},
                                         {ECubeType::TimeCube,    FChangeData{ 1, -1,  0}, true},
                                         {ECubeType::VeryBadCube, FChangeData{ 1,  0,  1}, true},
                                         {ECubeType::BadCube,     FChangeData{ 1,  0, -1}, false},
                                         {ECubeType::BonusCube,   FChangeData{ 1,  0,  0}, false},
                                         {ECubeType::GoodCube,    FChangeData{-1,  1,  1}, true},
                                         {ECubeType::ScoreCube,   FChangeData{-1,  1, -1}, true},
                                         {ECubeType::SpeedCube,   FChangeData{-1,  1,  0}, true},
                                         {ECubeType::TimeCube,    FChangeData{-1, -1,  1}, true},
                                         {ECubeType::VeryBadCube, FChangeData{-1, -1, -1}, true},
                                         {ECubeType::BadCube,     FChangeData{-1, -1,  0}, true},
                                         {ECubeType::BonusCube,   FChangeData{-1,  0,  1}, true},
                                         {ECubeType::GoodCube,    FChangeData{-1,  0, -1}, true},
                                         {ECubeType::ScoreCube,   FChangeData{-1,  0,  0}, true},
                                         {ECubeType::SpeedCube,   FChangeData{ 0,  1,  1}, true},
                                         {ECubeType::TimeCube,    FChangeData{ 0,  1, -1}, false},
                                         {ECubeType::VeryBadCube, FChangeData{ 0,  1,  0}, false},
                                         {ECubeType::BadCube,     FChangeData{ 0, -1,  1}, true},
                                         {ECubeType::BonusCube,   FChangeData{ 0, -1, -1}, true},
                                         {ECubeType::GoodCube,    FChangeData{ 0, -1,  0}, true},
                                         {ECubeType::ScoreCube,   FChangeData{ 0,  0,  1}, true},
                                         {ECubeType::SpeedCube,   FChangeData{ 0,  0, -1}, false},
                                         {ECubeType::TimeCube,    FChangeData{ 0,  0,  0}, false}};
                     // clang-format on
                     for (const auto& [CubeType, ChangeData, ExpectedValue] : TestPayload)
                     {
                         const FString CubeName{StaticEnum<ECubeType>()->GetNameStringByValue(static_cast<int64>(CubeType))};
                         It(FString::Printf(TEXT("CubeEffectShouldBeCorrectlyDefined.%s:Score(%d)Time(%d)Speed(%d)"),    //
                                            *CubeName,                                                                   //
                                            ChangeData.Score,                                                            //
                                            ChangeData.Time,                                                             //
                                            ChangeData.Speed),
                            [=, this]()
                            {
                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    //
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    //
                                                          ChangeData.ToString()});

                                TestTrueExpr(GameMode->IsCubeNegative(CubeType) == ExpectedValue);
                            });
                     }
                 }

                 AfterEach(
                     [this]()
                     {
                         SpecCloseLevel(World);
                     });
             });

    Describe("Framework.Pawn",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("/Game/Tests/TestLevel");
                         World = GetTestGameWorld();
                         TestNotNull("World should exist.", World);
                     });

                 It("PawnLocationShouldBeAdjustCorrectly",
                    [this]()
                    {
                        TestNotNull("GEngine should exist.", GEngine);
                        TestNotNull("GameViewport should exist.", GEngine->GameViewport.Get());

                        FSceneViewport* SceneViewport = GEngine->GameViewport->GetGameViewport();
                        TestNotNull("SceneViewport should exist.", SceneViewport);

                        const APlayerController* PC = World->GetFirstPlayerController();
                        TestNotNull("PlayerController should exist.", PC);

                        auto* PlayerPawn = Cast<ACGPlayer>(PC->GetPawn());
                        TestNotNull("PlayerPawn should exist.", PlayerPawn);

                        struct TestValue
                        {
                            FUintPoint ViewportSize;
                            FVector FieldSize;
                        };
                        const TArray<TestPayload<TestValue, FVector>> Payload{{{FUintPoint{600, 100}, FVector{1000.0, 400.0, 0.0}}, FVector{500.0, 200.0, 3900.0}},
                                                                              {{FUintPoint{2000, 100}, FVector{1200.0, 200.0, 0.0}}, FVector{600.0, 100.0, 15000.0}},
                                                                              {{FUintPoint{600, 100}, FVector{400.0, 1000.0, 0.0}}, FVector{200.0, 500.0, 2100.0}},
                                                                              {{FUintPoint{2000, 100}, FVector{200.0, 1200.0, 0.0}}, FVector{100.0, 600.0, 5000.0}},
                                                                              {{FUintPoint{100, 600}, FVector{1000.0, 400.0, 0.0}}, FVector{500.0, 200.0, 350.0}},
                                                                              {{FUintPoint{100, 2000}, FVector{1200.0, 200.0, 0.0}}, FVector{600.0, 100.0, 250.0}},
                                                                              {{FUintPoint{100, 600}, FVector{400.0, 1000.0, 0.0}}, FVector{200.0, 500.0, 650.0}},
                                                                              {{FUintPoint{100, 2000}, FVector{200.0, 1200.0, 0.0}}, FVector{100.0, 600.0, 750.0}},
                                                                              {{FUintPoint{600, 600}, FVector{1000.0, 400.0, 0.0}}, FVector{500.0, 200.0, 650.0}},
                                                                              {{FUintPoint{600, 600}, FVector{200.0, 1200.0, 0.0}}, FVector{100.0, 600.0, 750.0}},
                                                                              {{FUintPoint{2000, 2000}, FVector{1000.0, 400.0, 0.0}}, FVector{500.0, 200.0, 650.0}},
                                                                              {{FUintPoint{2000, 2000}, FVector{200.0, 1200.0, 0.0}}, FVector{100.0, 600.0, 750.0}}};

                        for (const auto& OnePayload : Payload)
                        {
                            PlayerPawn->UpdateLocation(FTransform::Identity, OnePayload.TestValue.FieldSize);

                            const FUintPoint ViewportSize = OnePayload.TestValue.ViewportSize;
                            SceneViewport->SetViewportSize(ViewportSize.X, ViewportSize.Y);

                            TestTrueExpr(PlayerPawn->GetActorLocation().Equals(OnePayload.ExpectedValue));
                        }
                    });

                 AfterEach(
                     [this]()
                     {
                         SpecCloseLevel(World);
                     });
             });

    Describe("Framework.GameUserSettings",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         GameUserSettings = NewObject<UCGGameUserSettings>();
                         TestNotNull("GameUserSettings should exist.", GameUserSettings);
                     });

                 It("AllDifficultyDisplayNamesShouldExist",
                    [this]()
                    {
                        const UEnum* DifficultyEnum = StaticEnum<EDifficulty>();
                        for (int32 i = 0; i < DifficultyEnum->NumEnums() - 2; ++i)
                        {
                            TestTrueExpr(!GameUserSettings->GetDifficultyDisplayName(static_cast<EDifficulty>(i)).IsEmpty());
                        }
                    });

                 It("DifficultyShouldBeSaved",
                    [this]()
                    {
                        const UEnum* DifficultyEnum = StaticEnum<EDifficulty>();
                        for (int32 i = 0; i < DifficultyEnum->NumEnums() - 2; ++i)
                        {
                            const auto Difficulty = static_cast<EDifficulty>(i);
                            GameUserSettings->SetDifficulty(Difficulty);
                            TestTrueExpr(GameUserSettings->GetCurrentDifficulty() == Difficulty);
                        }
                    });
             });
}

#endif
