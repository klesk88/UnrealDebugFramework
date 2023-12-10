using System.IO;
using UnrealBuildTool;

public class KLDebugImGuiNetworking : ModuleRules
{
    public KLDebugImGuiNetworking(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugImGuiFramework",
                "KLDebugImGuiUser",
                "KLDebugNetworkingRuntime",
                "KLDebugUtils",

                //engine
                "DeveloperSettings",
                "Engine",
                "Networking",
                "Sockets"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject"
            }
        );

        if (Target.bBuildWithEditorOnlyData)
        {
            PublicDependencyModuleNames.AddRange(new string[] {
                "KLDebugNetworkingArbitrer" ///to check in the settings for the port
            });
        }
    }
}
