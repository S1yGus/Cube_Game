#include "CGUtils.h"

UWorld* CGUtils::GetCurrentWorld()
{
    if (!GEngine)
        return nullptr;

    for (const auto& Context : GEngine->GetWorldContexts())
    {
        switch (Context.WorldType)
        {
            case EWorldType::PIE:
                return Context.World();
            case EWorldType::GamePreview:
                return Context.World();
        }
    }

    return nullptr;
}
