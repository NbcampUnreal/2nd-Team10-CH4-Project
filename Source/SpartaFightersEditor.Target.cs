

using UnrealBuildTool;
using System.Collections.Generic;

public class SpartaFightersEditorTarget : TargetRules
{
	public SpartaFightersEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "SpartaFighters" } );
	}
}
