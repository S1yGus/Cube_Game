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
    const FVideoSettings& GetVideoSettings() const { return VideoSettings; };
    void SetVideoSettings(const FVideoSettings& NewVideoSettings) { VideoSettings = NewVideoSettings; }

    const FSoundSettings& GetSoundSettings() const { return SoundSettings; };
    void SetSoundSettings(const FSoundSettings& NewSoundSettings) { SoundSettings = NewSoundSettings; }

    const FGameSettings& GetGameSettings() const { return GameSettings; };
    void SetGameSettings(const FGameSettings& NewGameSettings) { GameSettings = NewGameSettings; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FVideoSettings VideoSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FSoundSettings SoundSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FGameSettings GameSettings;
};
