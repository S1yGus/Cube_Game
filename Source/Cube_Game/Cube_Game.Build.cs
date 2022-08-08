// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Cube_Game : ModuleRules
{
    public Cube_Game(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "SlateCore", "Niagara" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
