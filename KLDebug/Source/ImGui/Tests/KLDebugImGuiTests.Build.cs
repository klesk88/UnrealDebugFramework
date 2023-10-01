using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugImGuiTests : ModuleRules
{
    public KLDebugImGuiTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
            new string[]
            {
                Path.Combine(ModuleDirectory, "Private"),
                Path.Combine(ModuleDirectory, "../Framework/Private"),
                Path.Combine(ModuleDirectory, "../User/Private"),
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "KLDebugImGuiFramework",
                "KLDebugImGuiUser",
                "ImGuiThirdParty",

                //engine
                "AITestSuite",
                "Engine"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AIModule",
                "Core",
                "CoreUObject"
            }
         );
    }
}
