// Cube_Game, All rights reserved.

#include "Settings/CGActionSetting.h"

void UCGActionSetting::DoAction()
{
    if (!Action)
        return;

    Action();
}

bool UCGActionSetting::IsActionAvailable()
{
    if (!Status)
        return false;

    return Status();
}
