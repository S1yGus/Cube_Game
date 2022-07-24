// Cube_Game, All rights reserved.

#pragma once

#include "CGSettingWidgetInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCGSettingWidgetInterface : public UInterface
{
    GENERATED_BODY()
};

class ICGSettingWidgetInterface
{
    GENERATED_BODY()

public:
    virtual void Update() = 0;
};
