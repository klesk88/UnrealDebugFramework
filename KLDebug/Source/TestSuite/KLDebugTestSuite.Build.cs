using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugTestSuite : ModuleRules
{
    public KLDebugTestSuite(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
            new string[]
            {
                Path.Combine(ModuleDirectory, "Private"),
                Path.Combine(ModuleDirectory, "../ImGui/Framework/Private"),
                Path.Combine(ModuleDirectory, "../ImGui/User/Private"),
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
