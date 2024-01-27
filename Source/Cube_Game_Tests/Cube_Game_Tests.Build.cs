// Cube_Game, All rights reserved.

using UnrealBuildTool;

public class Cube_Game_Tests : ModuleRules
{
    public Cube_Game_Tests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "FunctionalTesting", "Cube_Game", "Niagara" });
        PublicIncludePaths.AddRange(new string[] { "Cube_Game_Tests/Public", "Cube_Game/Public" });
    }
}
