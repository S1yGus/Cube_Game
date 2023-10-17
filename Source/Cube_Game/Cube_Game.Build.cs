// Cube_Game, All rights reserved.

using UnrealBuildTool;

public class Cube_Game : ModuleRules
{
    public Cube_Game(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "SlateCore", "Niagara", "AudioMixer", "EnhancedInput" });

        PrivateDependencyModuleNames.AddRange(new string[] {});
    }
}
