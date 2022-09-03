#include "CGUtils.h"

UWorld* CGUtils::GetCurrentWorld()
{
    if (!GEngine)
        return nullptr;

    for (const auto& Context : GEngine->GetWorldContexts())
    {
        if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::GamePreview) && Context.World() != nullptr)
        {
            return Context.World();
        }
    }

    return nullptr;
}
