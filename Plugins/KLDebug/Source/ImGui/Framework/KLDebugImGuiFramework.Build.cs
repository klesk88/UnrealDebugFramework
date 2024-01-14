using System.IO;
using UnrealBuildTool;

public class KLDebugImGuiFramework : ModuleRules
{
    public KLDebugImGuiFramework(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugImGuiUser",
                "KLDebugUserFramework",
                "KLDebugUtils",
                "KLUnrealImGui",
                "NetImGuiThirdParty",
                "UnrealImGui",
				
                //engine
                "DeveloperSettings",
                "Engine",
                "InputCore"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
            }
        );
    }
}
