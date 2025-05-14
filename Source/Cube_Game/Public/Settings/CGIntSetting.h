// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/CGSetting.h"
#include "CGIntSetting.generated.h"

UCLASS()
class CUBE_GAME_API UCGIntSetting : public UCGSetting
{
    GENERATED_BODY()

public:
    [[nodiscard]] const TArray<FText>& GetOptions() const { return Options; }
    [[nodiscard]] int32 GetCurrentValue() const;
    void SetValue(int32 NewValue);

private:
    TArray<FText> Options;
    TFunction<int32()> Getter;
    TFunction<void(int32)> Setter;

    void SetOptions(const TArray<FText>& NewOptions) { Options = NewOptions; }
    void AddGetter(TFunction<int32()>&& Func) { Getter = MoveTemp(Func); }
    void AddSetter(TFunction<void(int32)>&& Func) { Setter = MoveTemp(Func); }

    friend class UCGGameUserSettings;
};
