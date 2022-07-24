// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CGSetting.generated.h"

UCLASS()
class CUBE_GAME_API UCGSetting : public UObject
{
    GENERATED_BODY()

public:
    const FText& GetName() const { return Name; }

private:
    FText Name;

    void SetName(const FText& NewName) { Name = NewName; }

    friend class UCGGameUserSettings;
};
