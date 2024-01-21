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
