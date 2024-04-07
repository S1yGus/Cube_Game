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

    UCGGameUserSettings();

    static UCGGameUserSettings* Get();
    const TArray<TObjectPtr<UCGSetting>>& GetVideoSettings() const { return VideoSettings; }
    const TArray<TObjectPtr<UCGSetting>>& GetSoundSettings() const { return SoundSettings; }
    const TArray<TObjectPtr<UCGSetting>>& GetGameSettings() const { return GameSettings; }
    EPopUpType GetPopUpType() const;
    bool AreHintsEnabled() const;
    const FHintsStatus& GetHintsStatus() const;
    void SetHintsStatus(const FHintsStatus& NewHintsMap);
    EDifficulty GetCurrentDifficulty() const { return CurrentDifficulty; }
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

    TArray<FText> GetScreenResolutions() const;
    void UpdateResolutionSetting();
    void SetAllVideoSettings(int32 NewValue);
    void SetLowestResolution();
    void SetSoundClassVolume(const FString& SoundClassName, float NewVolume);

    void CheckSettingsSave();

    FORCEINLINE TObjectPtr<UCGIntSetting> CreateIntSetting(FText&& Name, const TArray<FText>& Options, TArray<TObjectPtr<UCGSetting>>& AddTo);
    FORCEINLINE TObjectPtr<UCGFloatSetting> CreateFloatSetting(FText&& Name, TArray<TObjectPtr<UCGSetting>>& AddTo);
    FORCEINLINE TObjectPtr<UCGActionSetting> CreateActionSetting(FText&& Name, FText&& ActionName, TArray<TObjectPtr<UCGSetting>>& AddTo);

    template <IsSetting T>
    TObjectPtr<T> CreateSetting(FText&& Name, TArray<TObjectPtr<UCGSetting>>& AddTo)
    {
        TObjectPtr<T> Setting = NewObject<T>();
        check(Setting);
        Setting->SetName(MoveTemp(Name));
        AddTo.Add(Setting);
        return Setting;
    }
};
