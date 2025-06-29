// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/CGSetting.h"
#include "CGActionSetting.generated.h"

UCLASS()
class CUBE_GAME_API UCGActionSetting : public UCGSetting
{
    GENERATED_BODY()

public:
    void DoAction();
    [[nodiscard]] bool IsActionAvailable();
    [[nodiscard]] const FText& GetActionName() const { return ActionName; }

private:
    FText ActionName;
    TFunction<void()> Action;
    TFunction<bool()> Status;

    void SetActionName(const FText& NewActionName) { ActionName = NewActionName; }
    void AddActionFunc(TFunction<void()>&& Func) { Action = MoveTemp(Func); }
    void AddStatusFunc(TFunction<bool()>&& Func) { Status = MoveTemp(Func); }

    friend class UCGGameUserSettings;
};
