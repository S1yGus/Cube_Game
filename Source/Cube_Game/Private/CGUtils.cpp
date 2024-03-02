// Cube_Game, All rights reserved.

#include "CGUtils.h"

FVector CubeGame::Utils::GetMeshAABBBoxSize(const UStaticMesh* Mesh)
{
    if (Mesh)
    {
        const FBox Box = Mesh->GetBoundingBox();
        return Box.GetSize();
    }
    return FVector::ZeroVector;
}

float CubeGame::Utils::ComputeCubeSpeed(const FSpeedData& DifficultySpeedData, int32 CurrentGameSpeed)
{
    return DifficultySpeedData.InitialSpeed + DifficultySpeedData.SpeedMultiplier * CurrentGameSpeed;
}

EHintType CubeGame::Utils::CubeTypeToHintType(ECubeType CubeType)
{
    switch (CubeType)
    {
        case ECubeType::GoodCube:
            return EHintType::GoodCube;
        case ECubeType::BadCube:
            return EHintType::BadCube;
        case ECubeType::ScoreCube:
            return EHintType::ScoreCube;
        case ECubeType::TimeCube:
            return EHintType::TimeCube;
        case ECubeType::BonusCube:
            return EHintType::BonusCube;
        case ECubeType::SpeedCube:
            return EHintType::SpeedCube;
        case ECubeType::VeryBadCube:
            return EHintType::VeryBadCube;
        default:
            return EHintType::Max;
    }
}

bool CubeGame::Utils::IsCubeNegative(ECubeType CubeType, const FDifficulty& DifficultyData)
{
    if (DifficultyData.ScoreChangeMap.Contains(CubeType))
    {
        if (DifficultyData.ScoreChangeMap[CubeType] < 0)
        {
            return true;
        }
    }

    if (DifficultyData.TimeChangeMap.Contains(CubeType))
    {
        if (DifficultyData.TimeChangeMap[CubeType] < 0)
        {
            return true;
        }
    }

    if (DifficultyData.SpeedChangeMap.Contains(CubeType))
    {
        if (DifficultyData.SpeedChangeMap[CubeType] > 0)
        {
            return true;
        }
    }

    return false;
}
