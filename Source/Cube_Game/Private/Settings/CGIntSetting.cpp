// Cube_Game, All rights reserved.

#include "Settings/CGIntSetting.h"

int32 UCGIntSetting::GetCurrentValue() const
{
    if (!Getter)
        return INDEX_NONE;

    return Getter();
}

void UCGIntSetting::SetValue(int32 NewValue)
{
    if (!Setter)
        return;

    Setter(NewValue);
}
