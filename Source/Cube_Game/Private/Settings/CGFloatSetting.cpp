// Cube_Game, All rights reserved.

#include "Settings/CGFloatSetting.h"

float UCGFloatSetting::GetCurrentValue() const
{
    if (!Getter)
        return 0.0f;

    return Getter();
}

void UCGFloatSetting::SetValue(float NewValue)
{
    if (!Setter)
        return;

    Setter(NewValue);
}
