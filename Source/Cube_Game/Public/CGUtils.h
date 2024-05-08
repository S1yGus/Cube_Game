// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CGCoreTypes.h"

class UInputMappingContext;
class UInputAction;

namespace CubeGame
{

class CUBE_GAME_API Utils
{
public:
    Utils() = delete;

    static FVector GetMeshAABBBoxSize(const UStaticMesh* Mesh);
    static float ComputeCubeSpeed(const FSpeedData& DifficultySpeedData, int32 CurrentGameSpeed);
    static EHintType CubeTypeToHintType(ECubeType CubeType);
    static bool IsCubeNegative(ECubeType CubeType, const FDifficulty& DifficultyData);
    static bool IsCubeAffectsMetrics(ECubeType CubeType, const FDifficulty& DifficultyData);
    static FText GetDifficultyDisplayName(EDifficulty Difficylty);
    static FText GetActionKeyName(TObjectPtr<const UInputMappingContext> MappingContext, TObjectPtr<const UInputAction> Action);
};

}    // namespace CubeGame
