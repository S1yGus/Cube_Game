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

template <class T>
concept IsSetting = std::derived_from<T, UCGSetting>;

UCLASS()
class CUBE_GAME_API UCGGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    FOnResolutionChangedSignature OnResolutionChanged;
    FOnPopUpTypeChangedSignature OnPopUpTypeChanged;
    FOnHintSettingsChangedSignature OnHintSettingsChanged;
    FOnLanguageChangedSignature OnLanguageChanged;

    UCGGameUserSettings();

    [[nodiscard]] static UCGGameUserSettings* Get();
    [[nodiscard]] const TArray<TObjectPtr<UCGSetting>>& GetVideoSettings() const { return VideoSettings; }
    [[nodiscard]] const TArray<TObjectPtr<UCGSetting>>& GetSoundSettings() const { return SoundSettings; }
    [[nodiscard]] const TArray<TObjectPtr<UCGSetting>>& GetGameSettings() const { return GameSettings; }
    [[nodiscard]] bool IsFistLaunch() const;
    void FistLaunchDone();
    [[nodiscard]] EPopUpType GetPopUpType() const;
    [[nodiscard]] bool AreHintsEnabled() const;
    [[nodiscard]] const FHintsStatus& GetHintsStatus() const;
    void SetHintsStatus(const FHintsStatus& NewHintsMap);
    [[nodiscard]] EDifficulty GetCurrentDifficulty() const { return CurrentDifficulty; }
    void SetDifficulty(EDifficulty NewDifficylty) { CurrentDifficulty = NewDifficylty; }

    void SetLastConfirmedResolutionSettings();
    void InitSoundVolume();

    virtual void LoadSettings(bool bForceReload = false) override;
    virtual void SaveSettings() override;

private:
    EDifficulty CurrentDifficulty{EDifficulty::Normal};
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

    FORCEINLINE [[nodiscard]] TArray<FText> GetScreenResolutions() const;
    void UpdateResolutionSetting();
    void SetAllVideoSettings(int32 NewQuality);
    void SetLowestResolution();
    void SetSoundClassVolume(const FString& SoundClassName, float NewVolume);

    void CheckSettingsSave();

    FORCEINLINE [[nodiscard]] TObjectPtr<UCGIntSetting> CreateIntSetting(const FText& Name, const TArray<FText>& Options, TArray<TObjectPtr<UCGSetting>>& AddTo);
    FORCEINLINE [[nodiscard]] TObjectPtr<UCGFloatSetting> CreateFloatSetting(const FText& Name, TArray<TObjectPtr<UCGSetting>>& AddTo);
    FORCEINLINE [[nodiscard]] TObjectPtr<UCGActionSetting> CreateActionSetting(const FText& Name, const FText& ActionName, TArray<TObjectPtr<UCGSetting>>& AddTo);

    template <IsSetting T>
    TObjectPtr<T> CreateSetting(const FText& Name, TArray<TObjectPtr<UCGSetting>>& AddTo)
    {
        TObjectPtr<T> Setting = NewObject<T>();
        check(Setting);
        Setting->SetName(Name);
        AddTo.Add(Setting);
        return Setting;
    }
};
