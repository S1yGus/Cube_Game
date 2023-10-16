// Cube_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

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

void SpecCloseLevel(const UWorld* World);
void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);
UWorld* GetTestWorld();

template <typename T1, typename T2>
struct TestPayload
{
    T1 TestValue;
    T2 ExpectedValue;
};

template <class T>
T* SpawnBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    auto* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

template <class PropertyClass, class ObjectClass>
PropertyClass GetPropertyValueByName(ObjectClass* Obj, const FString& PropName)
{
    if (Obj)
    {
        for (TFieldIterator<FProperty> PropIt(Obj->StaticClass()); PropIt; ++PropIt)
        {
            const FProperty* Property = *PropIt;
            if (Property && Property->GetName().Equals(PropName))
            {
                return *Property->ContainerPtrToValuePtr<PropertyClass>(Obj);
            }
        }
    }

    return PropertyClass();
}

}    // namespace Test

#endif
