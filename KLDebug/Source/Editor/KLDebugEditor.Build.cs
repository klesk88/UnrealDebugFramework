using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugEditor : ModuleRules
{
    public KLDebugEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "ImGuiThirdParty",
                "ImPlotThirdParty",
                "KLDebugImGui",
                "KLDebugGameplay",
                "KLDebugUtils",
                "KLUnrealImGui",
                "NetImGuiThirdParty",
                "UnrealImGui",

                //engine
                "Engine",
                "EnhancedInput",
                "InputCore",
                "InputEditor",
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
