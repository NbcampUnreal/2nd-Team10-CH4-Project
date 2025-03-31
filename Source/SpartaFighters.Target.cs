

using UnrealBuildTool;
using System.Collections.Generic;

public class SpartaFightersTarget : TargetRules
{
	public SpartaFightersTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "SpartaFighters" } );
	}
}
