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
#include "CGUtils.h"

using namespace Test;

BEGIN_DEFINE_SPEC(FFramework, "CubeGame", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

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
                         AutomationOpenMap("/Game/Tests/TestLevel");
                         World = GetTestGameWorld();
                         if (!TestNotNull("World should exist.", World))
                             return;

                         GameMode = World->GetAuthGameMode<ACGGameMode>();
                         if (!TestNotNull("GameMode should exist.", GameMode))
                             return;

                         GameUserSettings = UCGGameUserSettings::Get();
                         TestNotNull("GameUserSettings should exist.", GameUserSettings);
                     });

                 {
                     const TArray<int32> TestPayload{0, 42, -42};
                     for (const auto Speed : TestPayload)
                     {
                         It(FString::Printf(TEXT("GameSpeedShouldBeChanged.Speed(%d)"), Speed),
                            [=, this]()
                            {
                                const int32 MaxMultiplier(3);
                                CallFuncByNameWithParams(GameMode, "SetMaxMultiplier", {FString::FromInt(MaxMultiplier)});
                                const auto CubeType{ECubeType::GoodCube};

                                if (Speed < 0)
                                {
                                    // Ensure that the speed value are sufficient for adding negative values
                                    CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                             {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                              FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                              FChangeData{.Speed = FMath::Abs(Speed)}.ToString(),                                // Speed
                                                              FString::FromInt(1)});                                                             // ScoreToSpeedUp

                                    for (int32 i = 0; i < MaxMultiplier; ++i)
                                    {
                                        GameMode->UpdateGameMetrics(CubeType);
                                    }

                                    // Reset Multiplier
                                    const auto AnotherCubeType{static_cast<ECubeType>((static_cast<int32>(CubeType) + 1) % static_cast<int32>(ECubeType::Max))};
                                    GameMode->UpdateGameMetrics(AnotherCubeType);
                                }

                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                          FChangeData{.Speed = Speed}.ToString(),                                            // Speed
                                                          FString::FromInt(1)});                                                             // ScoreToSpeedUp

                                for (int32 i = 1; i <= MaxMultiplier; ++i)
                                {
                                    const auto SpeedBefore = GameMode->GetGameSpeed();

                                    GameMode->UpdateGameMetrics(CubeType);

                                    TestTrueExpr(GameMode->GetGameSpeed() == SpeedBefore + Speed * i);
                                }
                            });
                     }
                 }

                 {
                     const TArray<int32> TestPayload{0, 42, -42};
                     for (const auto Time : TestPayload)
                     {
                         It(FString::Printf(TEXT("GameTimeShouldBeChanged.Time(%d)"), Time),
                            [=, this]()
                            {
                                const int32 MaxMultiplier(3);
                                CallFuncByNameWithParams(GameMode, "SetMaxMultiplier", {FString::FromInt(MaxMultiplier)});
                                const auto CubeType{ECubeType::GoodCube};

                                if (Time < 0)
                                {
                                    // Ensure that the tiem value are sufficient for adding negative values
                                    CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                             {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                              FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                              FChangeData{.Time = FMath::Abs(Time)}.ToString(),                                  // Time
                                                              FString::FromInt(1)});                                                             // ScoreToSpeedUp

                                    for (int32 i = 0; i < MaxMultiplier; ++i)
                                    {
                                        GameMode->UpdateGameMetrics(CubeType);
                                    }

                                    // Reset Multiplier
                                    const auto AnotherCubeType{static_cast<ECubeType>((static_cast<int32>(CubeType) + 1) % static_cast<int32>(ECubeType::Max))};
                                    GameMode->UpdateGameMetrics(AnotherCubeType);
                                }

                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                          FChangeData{.Time = Time}.ToString(),                                              // Time
                                                          FString::FromInt(1)});                                                             // ScoreToSpeedUp

                                for (int32 i = 1; i <= MaxMultiplier; ++i)
                                {
                                    const auto TimeBefore = GameMode->GetGameTime();

                                    GameMode->UpdateGameMetrics(CubeType);

                                    TestTrueExpr(GameMode->GetGameTime() == TimeBefore + Time * i);
                                }
                            });
                     }
                 }

                 {
                     const TArray<int32> TestPayload{0, 42, -42};
                     for (const auto Score : TestPayload)
                     {
                         It(FString::Printf(TEXT("GameScoreShouldBeChanged.Score(%d)"), Score),
                            [=, this]()
                            {
                                const int32 MaxMultiplier(3);
                                CallFuncByNameWithParams(GameMode, "SetMaxMultiplier", {FString::FromInt(MaxMultiplier)});
                                const auto CubeType{ECubeType::GoodCube};

                                if (Score < 0)
                                {
                                    // Ensure that the score value are sufficient for adding negative values
                                    CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                             {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                              FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                              FChangeData{.Score = FMath::Abs(Score)}.ToString(),                                // Score
                                                              FString::FromInt(1)});                                                             // ScoreToSpeedUp

                                    for (int32 i = 0; i < MaxMultiplier; ++i)
                                    {
                                        GameMode->UpdateGameMetrics(CubeType);
                                    }

                                    // Reset Multiplier
                                    const auto AnotherCubeType{static_cast<ECubeType>((static_cast<int32>(CubeType) + 1) % static_cast<int32>(ECubeType::Max))};
                                    GameMode->UpdateGameMetrics(AnotherCubeType);
                                }

                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                          FChangeData{.Score = Score}.ToString(),                                            // Score
                                                          FString::FromInt(1)});                                                             // ScoreToSpeedUp

                                for (int32 i = 1; i <= MaxMultiplier; ++i)
                                {
                                    const auto ScoreBefore = GameMode->GetScore();

                                    GameMode->UpdateGameMetrics(CubeType);

                                    TestTrueExpr(GameMode->GetScore() == ScoreBefore + Score * i);
                                }
                            });
                     }
                 }

                 {
                     using Payload = TArray<TTuple<int32, int32, int32>>;
                     const Payload TestPayload{{0, 10, 0}, {9, 10, 0}, {10, 10, 1}, {11, 10, 1}, {20, 10, 2}};
                     for (const auto [Score, ScoreToSpeedUp, AdditionalSpeed] : TestPayload)
                     {
                         It(FString::Printf(TEXT("GameSpeedShouldBeChangedDependingOnScore."
                                                 "Score(%d)ScoreToSpeedUp(%d)AdditionalSpeed(%d)"),
                                            Score, ScoreToSpeedUp, AdditionalSpeed),
                            [=, this]()
                            {
                                const auto SpeedBefore = GameMode->GetGameSpeed();
                                const auto CubeType{ECubeType::GoodCube};
                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                          FChangeData{.Score = Score}.ToString(),                                            // Score
                                                          FString::FromInt(ScoreToSpeedUp)});                                                // ScoreToSpeedUp

                                GameMode->UpdateGameMetrics(CubeType);

                                TestTrueExpr(GameMode->GetGameSpeed() == SpeedBefore + AdditionalSpeed);
                            });
                     }
                 }

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
                        const FHintsStatus NewHintsStatus = {{EHintType::Startup, false},        {EHintType::Combo, false},        {EHintType::TimeRunningOut, false},
                                                             {EHintType::SpeedIncreased, false}, {EHintType::BonusCharged, false}, {EHintType::GoodCube, false},
                                                             {EHintType::BadCube, false},        {EHintType::ScoreCube, false},    {EHintType::TimeCube, false},
                                                             {EHintType::BonusCube, false},      {EHintType::SpeedCube, false},    {EHintType::VeryBadCube, false}};
                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameMode->OnShowPopUpHint.AddLambda(
                                          [&](const FHintData&)
                                          {
                                              ++PopUpHintAmount;
                                          });

                                      GameUserSettings->OnHintSettingsChanged.Broadcast({true, NewHintsStatus});

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
                        const float ThreadSyncDelta{1.0f};
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
                        const FHintsStatus NewHintsStatus = {{EHintType::Startup, true},        {EHintType::Combo, true},        {EHintType::TimeRunningOut, true},
                                                             {EHintType::SpeedIncreased, true}, {EHintType::BonusCharged, true}, {EHintType::GoodCube, true},
                                                             {EHintType::BadCube, true},        {EHintType::ScoreCube, true},    {EHintType::TimeCube, true},
                                                             {EHintType::BonusCube, true},      {EHintType::SpeedCube, true},    {EHintType::VeryBadCube, true}};
                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameMode->OnShowPopUpHint.AddLambda(
                                          [&](const FHintData&)
                                          {
                                              ++PopUpHintAmount;
                                          });

                                      GameUserSettings->OnHintSettingsChanged.Broadcast({true, NewHintsStatus});

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
                        const float ThreadSyncDelta{1.0f};
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
                        const FHintsStatus NewHintsStatus = {{EHintType::Startup, true},        {EHintType::Combo, true},        {EHintType::TimeRunningOut, true},
                                                             {EHintType::SpeedIncreased, true}, {EHintType::BonusCharged, true}, {EHintType::GoodCube, false},
                                                             {EHintType::BadCube, true},        {EHintType::ScoreCube, true},    {EHintType::TimeCube, true},
                                                             {EHintType::BonusCube, true},      {EHintType::SpeedCube, true},    {EHintType::VeryBadCube, true}};
                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameMode->OnShowPopUpHint.AddLambda(
                                          [&](const FHintData&)
                                          {
                                              ++PopUpHintAmount;
                                          });

                                      GameUserSettings->OnHintSettingsChanged.Broadcast({true, NewHintsStatus});

                                      GameMode->EnqueueHint(ECubeType::GoodCube);
                                      GameMode->EnqueueHint(ECubeType::GoodCube);
                                      GameMode->EnqueueHint(ECubeType::GoodCube);
                                  });

                        const float NextHintDelay = GetPropertyValueByName<float>(GameMode, "NextHintDelay");
                        const float ThreadSyncDelta{1.0f};
                        FPlatformProcess::Sleep(NextHintDelay * 3 + ThreadSyncDelta);

                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameUserSettings->SetHintsStatus(OldHintsStatus);
                                  });
                        FPlatformProcess::Sleep(ThreadSyncDelta);

                        TestTrueExpr(PopUpHintAmount == 1);
                    });

                 It("DisabledHintsShouldNotBeShown", EAsyncExecution::ThreadPool,
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
                        const FHintsStatus NewHintsStatus = {{EHintType::Startup, false},        {EHintType::Combo, false},        {EHintType::TimeRunningOut, false},
                                                             {EHintType::SpeedIncreased, false}, {EHintType::BonusCharged, false}, {EHintType::GoodCube, false},
                                                             {EHintType::BadCube, false},        {EHintType::ScoreCube, false},    {EHintType::TimeCube, false},
                                                             {EHintType::BonusCube, false},      {EHintType::SpeedCube, false},    {EHintType::VeryBadCube, false}};
                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameMode->OnShowPopUpHint.AddLambda(
                                          [&](const FHintData&)
                                          {
                                              ++PopUpHintAmount;
                                          });

                                      GameUserSettings->OnHintSettingsChanged.Broadcast({false, NewHintsStatus});

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
                        const float ThreadSyncDelta{1.0f};
                        FPlatformProcess::Sleep(FMath::Max(NextHintDelay * NewHintsStatus.Num(), StartupHintDelay) + ThreadSyncDelta);

                        AsyncTask(ENamedThreads::GameThread,
                                  [&]()
                                  {
                                      GameUserSettings->SetHintsStatus(OldHintsStatus);
                                  });
                        FPlatformProcess::Sleep(ThreadSyncDelta);

                        TestTrueExpr(PopUpHintAmount == 0);
                    });

                 {
                     using Payload = TArray<TTuple<ECubeType, FChangeData, bool>>;
                     // clang-format off
                     const Payload TestPayload{{ECubeType::BadCube,     FChangeData{ 1,  1,  1}, true},
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
                         It(FString::Printf(TEXT("CubeEffectShouldBeCorrectlyDefined."
                                                 "%s:Score(%d)Time(%d)Speed(%d)"),    //
                                            *CubeName,                                //
                                            ChangeData.Score,                         //
                                            ChangeData.Time,                          //
                                            ChangeData.Speed),
                            [=, this]()
                            {
                                CallFuncByNameWithParams(GameMode, "SetTestDifficultyData",
                                                         {FString::FromInt(static_cast<int32>(GameUserSettings->GetCurrentDifficulty())),    // Difficulty
                                                          FString::FromInt(static_cast<int32>(CubeType)),                                    // CubeType
                                                          ChangeData.ToString(),                                                             // ChangeData
                                                          FString::FromInt(1)});                                                             // ScoreToSpeedUp

                                TestTrueExpr(CubeGame::Utils::IsCubeNegative(CubeType, GameMode->GetDifficultyData()) == ExpectedValue);
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
                        if (!TestNotNull("GEngine should exist.", GEngine))
                            return;

                        if (!TestNotNull("GameViewport should exist.", GEngine->GameViewport.Get()))
                            return;

                        FSceneViewport* SceneViewport = GEngine->GameViewport->GetGameViewport();
                        if (!TestNotNull("SceneViewport should exist.", SceneViewport))
                            return;

                        const APlayerController* PC = World->GetFirstPlayerController();
                        if (!TestNotNull("PlayerController should exist.", PC))
                            return;

                        auto* PlayerPawn = Cast<ACGPlayer>(PC->GetPawn());
                        if (!TestNotNull("PlayerPawn should exist.", PlayerPawn))
                            return;

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
                            TestTrueExpr(!CubeGame::Utils::GetDifficultyDisplayName(static_cast<EDifficulty>(i)).IsEmpty());
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
