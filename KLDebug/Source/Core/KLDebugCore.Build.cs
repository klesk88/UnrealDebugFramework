using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugCore : ModuleRules
{
    public KLDebugCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
            new string[]
            {
                Path.Combine(ModuleDirectory, "Private"),
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Engine"
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject"
            }
         );

        if (Target.bBuildDeveloperTools || Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            if(Target.Configuration == UnrealTargetConfiguration.Test)
            {
                PublicDefinitions.Add("WITH_KL_TEST_DEBUGGER=1");
            }
            else
            {
                PublicDefinitions.Add("WITH_KL_DEBUGGER=1");
            }
        }
        else
        {
            PublicDefinitions.Add("WITH_KL_DEBUGGER=0");
            PublicDefinitions.Add("WITH_KL_TEST_DEBUGGER=0");
        }
    }
}
