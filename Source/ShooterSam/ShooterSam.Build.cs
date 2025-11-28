// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShooterSam : ModuleRules
{
	public ShooterSam(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ShooterSam",
			"ShooterSam/StarterScripts/Variant_Platforming",
			"ShooterSam/StarterScripts/Variant_Platforming/Animation",
			"ShooterSam/StarterScripts/Variant_Combat",
			"ShooterSam/StarterScripts/Variant_Combat/AI",
			"ShooterSam/StarterScripts/Variant_Combat/Animation",
			"ShooterSam/StarterScripts/Variant_Combat/Gameplay",
			"ShooterSam/StarterScripts/Variant_Combat/Interfaces",
			"ShooterSam/StarterScripts/Variant_Combat/UI",
			"ShooterSam/StarterScripts/Variant_SideScrolling",
			"ShooterSam/StarterScripts/Variant_SideScrolling/AI",
			"ShooterSam/StarterScripts/Variant_SideScrolling/Gameplay",
			"ShooterSam/StarterScripts/Variant_SideScrolling/Interfaces",
			"ShooterSam/StarterScripts/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
