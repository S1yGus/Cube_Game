// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/CGSetting.h"
#include "CGFloatSetting.generated.h"

UCLASS()
class CUBE_GAME_API UCGFloatSetting : public UCGSetting
{
    GENERATED_BODY()

public:
    [[nodiscard]] float GetCurrentValue() const;
    void SetValue(float NewValue);

private:
    TFunction<float()> Getter;
    TFunction<void(float)> Setter;

    void AddGetter(TFunction<float()>&& Func) { Getter = MoveTemp(Func); }
    void AddSetter(TFunction<void(float)>&& Func) { Setter = MoveTemp(Func); }

    friend class UCGGameUserSettings;
};
