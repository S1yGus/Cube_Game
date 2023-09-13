// Cube_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/CGTestUtils.h"
#include "Misc/OutputDeviceNull.h"

void Test::SpecCloseLevel(const UWorld* World)
{
    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        PC->ConsoleCommand(TEXT("Exit"), true);
    }
}

void Test::CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    if (Object)
    {
        FString Command = FuncName;
        for (const auto& Param : Params)
        {
            Command.Append(" ").Append(Param);
        }
        FOutputDeviceNull OutputDeviceNull;
        Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
    }
}

UWorld* Test::GetTestWorld()
{
    if (GEngine)
    {
        for (const auto& Context : GEngine->GetWorldContexts())
        {
            if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::GamePreview)
            {
                return Context.World();
            }
        }
    }

    return nullptr;
}

#endif
