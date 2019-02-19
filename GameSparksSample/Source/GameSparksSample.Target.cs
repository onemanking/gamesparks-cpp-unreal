// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class GameSparksSampleTarget : TargetRules
{
    public GameSparksSampleTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Game;
        _SetupBinaries(ref ExtraModuleNames); // remove this line when using the plugin pre 4.16
	}

    // SetupBinaries() is deprecated in the 4.16 release.
    // to use with pre-4.16, switch the commenting in the next two lines and remove the call in the constructor
    //public override void SetupBinaries(TargetInfo Target, ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations, ref List<string> OutExtraModuleNames
    public void _SetupBinaries(ref List<string> OutExtraModuleNames)
	{
		OutExtraModuleNames.AddRange( new string[] { "GameSparksSample" } );
	}
}
