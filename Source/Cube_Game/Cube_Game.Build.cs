// Cube_Game, All rights reserved.

using System.Linq;
using UnrealBuildTool;

public class Cube_Game : ModuleRules
{
    public Cube_Game(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.ProjectDefinitions.Contains("UNOPTIMIZED_CODE"))
        {
            OptimizeCode = CodeOptimization.Never;
        }

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "SlateCore", "Niagara", "AudioMixer", "EnhancedInput", "AudioSynesthesia" });
        PublicIncludePaths.AddRange(new string[] { "Cube_Game/Public" });
    }
}
