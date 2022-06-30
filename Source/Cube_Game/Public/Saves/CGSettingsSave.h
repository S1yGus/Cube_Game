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

public:
    inline const FVideoSettings& GetVideoSettings() const { return VideoSettings; };
    inline void SetVideoSettings(const FVideoSettings& NewVideoSettings) { VideoSettings = NewVideoSettings; }

    inline const FSoundSettings& GetSoundSettings() const { return SoundSettings; };
    inline void SetSoundSettings(const FSoundSettings& NewSoundSettings) { SoundSettings = NewSoundSettings; }

    inline const FGameSettings& GetGameSettings() const { return GameSettings; };
    inline void SetGameSettings(const FGameSettings& NewGameSettings) { GameSettings = NewGameSettings; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FVideoSettings VideoSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FSoundSettings SoundSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FGameSettings GameSettings;
};
