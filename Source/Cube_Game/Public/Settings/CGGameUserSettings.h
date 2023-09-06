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
    FOnMusicTypeChangedSignature OnMusicTypeChanged;

    UCGGameUserSettings();

    static UCGGameUserSettings* Get();
    const TArray<TObjectPtr<UCGSetting>>& GetVideoSettings() const { return VideoSettings; }
    const TArray<TObjectPtr<UCGSetting>>& GetSoundSettings() const { return SoundSettings; }
    const TArray<TObjectPtr<UCGSetting>>& GetGameSettings() const { return GameSettings; }
    EPopUpType GetPopUpType() const;
    const FHintsStatus& GetHintsStatus() const;
    const FAspectRatioData& GetAspectRatio() const;
    bool GetMusicType() const;
    void SetGameplayHintsStatus(const TMap<EHintType, bool>& NewHintsMap);
    void SetCollectHintsStatus(const TMap<ECubeType, bool>& NewHintsMap);
    void SetLastConfirmedResolutionSettings();

    void InitSoundVolume();

    virtual void LoadSettings(bool bForceReload = false) override;
    virtual void SaveSettings() override;

private:
    UPROPERTY()
    TArray<TObjectPtr<UCGSetting>> VideoSettings;
    UPROPERTY()
    TObjectPtr<UCGIntSetting> ResolutionSetting;
    UPROPERTY()
    TArray<TObjectPtr<UCGSetting>> SoundSettings;
    UPROPERTY()
    TArray<TObjectPtr<UCGSetting>> GameSettings;

    UPROPERTY()
    TObjectPtr<UCGSettingsSave> SettingsSave;

    void InitVideoSettings();
    void InitSoundSettings();
    void InitGameSettings();

    TArray<FText> GetScreenResolutions() const;
    void UpdateResolutionSetting();
    void SetAllVideoSettings(int32 NewValue);
    void SetLowestResolution();
    void SetSoundClassVolume(const FString& SoundClassName, float NewVolume);

    void CheckSettingsSave();

    FORCEINLINE TObjectPtr<UCGIntSetting> CreateIntSetting(const FText& Name, const TArray<FText>& Options, TArray<TObjectPtr<UCGSetting>>& AddTo);
    FORCEINLINE TObjectPtr<UCGFloatSetting> CreateFloatSetting(const FText& Name, TArray<TObjectPtr<UCGSetting>>& AddTo);
    FORCEINLINE TObjectPtr<UCGActionSetting> CreateActionSetting(const FText& Name, const FText& ActionName, TArray<TObjectPtr<UCGSetting>>& AddTo);

    template <class T>
    TObjectPtr<T> CreateSetting(const FText& Name, TArray<TObjectPtr<UCGSetting>>& AddTo)
    {
        TObjectPtr<T> Setting = NewObject<T>();
        checkf(Setting->IsA(UCGSetting::StaticClass()), TEXT("T must be based on UCGSetting"));
        Setting->SetName(Name);
        AddTo.Add(Setting);

        return Setting;
    }
};
