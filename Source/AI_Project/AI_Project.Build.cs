// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AI_Project : ModuleRules
{
	public AI_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",  "AIModule", "NavigationSystem", "GameplayAbilities", "GameplayTasks", "GameplayTags" });
	}
}
