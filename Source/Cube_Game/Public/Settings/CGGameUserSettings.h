// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CGCoreTypes.h"
#include "CGGameUserSettings.generated.h"

class UCGSetting;
class UCGIntSetting;
class UCGFloatSetting;
class UCGActionSetting;
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
    inline const TArray<UCGSetting*>& GetVideoSettings() const { return VideoSettings; }
    inline const TArray<UCGSetting*>& GetSoundSettings() const { return SoundSettings; }
    inline const TArray<UCGSetting*>& GetGameSettings() const { return GameSettings; }
    EPopUpType GetPopUpType() const;
    const FHintsStatus& GetHintsStatus() const;
    const FAspectRatioData& GetAspectRatio() const;
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

    inline UCGIntSetting* CreateIntSetting(const FText& Name, const TArray<FText>& Options, TArray<UCGSetting*>& AddTo);
    inline UCGFloatSetting* CreateFloatSetting(const FText& Name, TArray<UCGSetting*>& AddTo);
    inline UCGActionSetting* CreateActionSetting(const FText& Name, const FText& ActionName, TArray<UCGSetting*>& AddTo);

    template <class T>
    T* CreateSetting(const FText& Name, TArray<UCGSetting*>& AddTo)
    {
        T* Setting = NewObject<T>();
        checkf(Setting->IsA(UCGSetting::StaticClass()), TEXT("T must be based on UCGSetting"));
        Setting->SetName(Name);
        AddTo.Add(Setting);

        return Setting;
    }
};
