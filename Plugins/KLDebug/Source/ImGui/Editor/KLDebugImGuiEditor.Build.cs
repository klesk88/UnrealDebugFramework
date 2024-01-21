using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugImGuiEditor : ModuleRules
{
    public KLDebugImGuiEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugUserThirdPartyImGui",
                "KLDebugUserThirdPartyImGuiNodeEditor",
                "KLDebugUserThirdPartyImPlot",
                "KLDebugImGuiFramework",
                "KLDebugUtils",
                "KLUnrealImGui",
                "NetImGuiThirdParty",
                "UnrealImGui",

                //engine
                "Engine",
                "LevelEditor",
                "Projects"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
                "UnrealEd"
            }
         );
    }
}
