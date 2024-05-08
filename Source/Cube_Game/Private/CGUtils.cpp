// Cube_Game, All rights reserved.

#include "CGUtils.h"
#include "InputMappingContext.h"
#include "InputAction.h"

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

bool CubeGame::Utils::IsCubeAffectsMetrics(ECubeType CubeType, const FDifficulty& DifficultyData)
{
    return DifficultyData.ScoreChangeMap.Contains(CubeType) || DifficultyData.TimeChangeMap.Contains(CubeType) || DifficultyData.SpeedChangeMap.Contains(CubeType);
}

FText CubeGame::Utils::GetDifficultyDisplayName(EDifficulty Difficylty)
{
    switch (Difficylty)
    {
        case EDifficulty::Normal:
            return NSLOCTEXT("Utils", "DifficultyNormal_Loc", "Normal");
        case EDifficulty::Hard:
            return NSLOCTEXT("Utils", "DifficultyHard_Loc", "Hard");
        default:
            return FText::GetEmpty();
    }
}

FText CubeGame::Utils::GetActionKeyName(TObjectPtr<const UInputMappingContext> MappingContext, TObjectPtr<const UInputAction> Action)
{
    auto* FoundMapping = MappingContext->GetMappings().FindByPredicate(
        [&](const FEnhancedActionKeyMapping& ActionKeyMapping)
        {
            return ActionKeyMapping.Action == Action;
        });

    return FoundMapping ? FoundMapping->Key.GetDisplayName() : FText::AsCultureInvariant("");
}
