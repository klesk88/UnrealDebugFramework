using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugExamples : ModuleRules
{
    public KLDebugExamples(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
            new string[]
            {
                Path.Combine(ModuleDirectory, "Private")
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "ImGuiThirdParty",
                "ImPlotThirdParty",
                "KLDebugImGuiFramework",
                "KLDebugImGuiUser",

                //engine
                "AIModule",
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
    }
}
