

using UnrealBuildTool;

public class SpartaFighters : ModuleRules
{
	public SpartaFighters(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "NetCore",
            "EnhancedInput",
            "UMG",
            "HTTP",
            "JsonUtilities",
            "Json",
            "AIModule",
            "GamePlayTasks",
            "NavigationSystem",
            "Niagara",
            "GameplayTags"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore"
        });

        PublicIncludePaths.AddRange(new string[]
        {
            "SpartaFighters"
        });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
