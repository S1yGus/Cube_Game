// Cube_Game, All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Cube_GameTarget : TargetRules
{
    public Cube_GameTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "Cube_Game" });
    }
}
