// Cube_Game, All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using EpicGames.Core;

public class Cube_GameTarget : TargetRules
{
    [CommandLine("-UnoptimizedCode")]
    public bool UnoptimizedCode = false;

    public Cube_GameTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "Cube_Game" });

        if (UnoptimizedCode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }
    }
}
