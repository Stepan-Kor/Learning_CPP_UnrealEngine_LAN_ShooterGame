// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShooterCPP : ModuleRules
{
	public ShooterCPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
		"AIModule"});
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubSystem", "OnlineSubSystemNull", "OnlineSubsystemSteam" });

	}
}
