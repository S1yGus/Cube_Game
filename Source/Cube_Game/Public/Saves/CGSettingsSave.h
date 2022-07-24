// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CGCoreTypes.h"
#include "CGSettingsSave.generated.h"

UCLASS()
class CUBE_GAME_API UCGSettingsSave : public USaveGame
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FVideoSettings VideoSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FSoundSettings SoundSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FGameSettings GameSettings;

    friend class UCGGameUserSettings;
};
