// Cube_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CGCoreTypes.h"

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
};

}    // namespace CubeGame
