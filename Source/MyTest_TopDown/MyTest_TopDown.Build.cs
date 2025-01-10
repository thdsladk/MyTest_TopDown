// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyTest_TopDown : ModuleRules
{
	public MyTest_TopDown(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput","UMG","NavigationSystem","AIModule","GameplayTasks","SlateCore" });

		PrivateIncludePaths.AddRange(new string[]
        {
			"MyTest_TopDown",
			"MyTest_TopDown/Default",
			"MyTest_TopDown/Character",
			"MyTest_TopDown/AI",
			"MyTest_TopDown/Monster",
			"MyTest_TopDown/Item",
			"MyTest_TopDown/UI",
			"MyTest_TopDown/Data",
			"MyTest_TopDown/Interface",
			"MyTest_TopDown/Skill",
			"MyTest_TopDown/Physics"

		});
	}
}
