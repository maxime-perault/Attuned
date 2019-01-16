// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class AttunedTool : ModuleRules
{
	public AttunedTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange (new string[] { Path.Combine(ModuleDirectory, "Public/")  });
		PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private/") });

        PublicDependencyModuleNames.AddRange (
            new string[]
            {
                "Core",
                "Slate",
                "SlateCore",
                "Attuned"
            }
            );

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"UnrealEd",
				"LevelEditor",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Attuned"
			}
			);

		DynamicallyLoadedModuleNames.AddRange(new string[]{});
    }
}
