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
                Path.Combine(ModuleDirectory, "../ImGui/Private"),
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "KLDebugImGui",
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
