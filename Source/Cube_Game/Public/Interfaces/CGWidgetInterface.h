#pragma once

#include "CGWidgetInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCGWidgetInterface : public UInterface
{
    GENERATED_BODY()
};

class ICGWidgetInterface
{
    GENERATED_BODY()

public:
    virtual void ResetWidget() = 0;
};