// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPS_Multiplay : ModuleRules
{
	public FPS_Multiplay(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "MoviePlayer" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "PlayFab", "PlayFabCpp", "PlayFabCommon" });
	}
}
