// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GameSparksSample : ModuleRules
{
    // this API changed in UE 4.15, uncomment the following, if you need to support earlier versions
    // public GameSparksSample(TargetInfo Target)
    public GameSparksSample(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameSparks", "OnlineSubsystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you want to use the GameSparksOnlineSubsystem
		PrivateDependencyModuleNames.Add("OnlineSubsystem");
		PrivateDependencyModuleNames.Add("OnlineSubsystemGameSparks");
	}
}
