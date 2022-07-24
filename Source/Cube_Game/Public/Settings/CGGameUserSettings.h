// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CGCoreTypes.h"
#include "CGGameUserSettings.generated.h"

class UCGSetting;
class UCGIntSetting;
class UCGSettingsSave;

UCLASS()
class CUBE_GAME_API UCGGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    FOnResolutionChangedSignature OnResolutionChanged;
    FOnPopUpTypeChangedSignature OnPopUpTypeChanged;
    FOnHintsStatusChangedSignature OnHintsStatusChanged;
    FOnAspectRatioChangedSignature OnAspectRatioChanged;

    UCGGameUserSettings();

    static UCGGameUserSettings* Get();
    const TArray<UCGSetting*>& GetVideoSettings() const { return VideoSettings; }
    const TArray<UCGSetting*>& GetSoundSettings() const { return SoundSettings; }
    const TArray<UCGSetting*>& GetGameSettings() const { return GameSettings; }
    EPopUpType GetPopUpType() const;
    const FHintsStatus& GetHintsStatus() const;
    void SetGameplayHintsStatus(const TMap<EHintType, bool>& NewHintsMap);
    void SetReceivingHintsStatus(const TMap<ECubeType, bool>& NewHintsMap);
    void SetLastConfirmedResolutionSettings();

    void InitSoundVolume();

    virtual void LoadSettings(bool bForceReload = false) override;
    virtual void SaveSettings() override;

private:
    UPROPERTY()
    TArray<UCGSetting*> VideoSettings;
    UPROPERTY()
    UCGIntSetting* ResolutionSetting;
    UPROPERTY()
    TArray<UCGSetting*> SoundSettings;
    UPROPERTY()
    TArray<UCGSetting*> GameSettings;

    UPROPERTY()
    UCGSettingsSave* SettingsSave;

    void InitVideoSettings();
    void InitSoundSettings();
    void InitGameSettings();

    const TArray<FText> GetScreenResolutions() const;
    void UpdateResolutionSetting();
    void SetAllVideoSettings(int32 NewValue);
    void SetLowestResolution();
    void SetSoundClassVolume(const FString& SoundClassName, float NewVolume);

    void CheckSettingsSave();
};
