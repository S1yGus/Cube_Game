// Cube_Game, All rights reserved.

#pragma once

#include "CGTestUtils.generated.h"

USTRUCT(BlueprintType)
struct FChangeData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 Score{0};

    UPROPERTY(BlueprintReadWrite, Meta = (ClampMin = "0", Units = "s"))
    int32 Time{0};

    UPROPERTY(BlueprintReadWrite)
    int32 Speed{0};

    FString ToString() const { return FString::Printf(TEXT("(Score=%d,Time=%d,Speed=%d)"), Score, Time, Speed); }
};
