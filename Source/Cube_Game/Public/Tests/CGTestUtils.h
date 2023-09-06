#pragma once

#include "CoreMinimal.h"
#include "Tests/AutomationCommon.h"
#include "Engine/Blueprint.h"

namespace Test
{

class LevelScope
{
public:
    LevelScope(const FString& LevelName) { AutomationOpenMap(LevelName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

template <class T>
T* SpawnBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    auto* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

}    // namespace Test
