using UnrealBuildTool;

public class MultiplayerFPS : ModuleRules
{
	public MultiplayerFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "MoviePlayer" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "PlayFab", "PlayFabCpp", "PlayFabCommon" });
	}
}
