// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SinkDownProject : ModuleRules
{
	public SinkDownProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "UMG", "Niagara", "MediaAssets", "Media" });
    }
}
