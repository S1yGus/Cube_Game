// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGCoreTypes.h"
#include "CGGameInstance.generated.h"

UCLASS()
class CUBE_GAME_API UCGGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    FOnAspectRatioChangedSignature OnAspectRatioChanged;

    void SetDifficulty(EDifficulty NewDifficylty) { CurrentDifficulty = NewDifficylty; }
    EDifficulty GetDifficulty() const { return CurrentDifficulty; }

private:
    EDifficulty CurrentDifficulty = EDifficulty::Medium;
};
