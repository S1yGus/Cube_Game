// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/CGHUDBase.h"
#include "CGHUDCredits.generated.h"

UCLASS()
class CUBE_GAME_API ACGHUDCredits : public ACGHUDBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UUserWidget> AuthorsWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<UUserWidget> TitleWidgetClass;

    virtual void BeginPlay() override;

private:
    virtual void SetupWidgets() override;
};
