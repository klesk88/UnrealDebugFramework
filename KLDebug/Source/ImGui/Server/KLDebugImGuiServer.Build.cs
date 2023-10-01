using System.IO;
using UnrealBuildTool;

public class KLDebugImGuiServer : ModuleRules
{
    public KLDebugImGuiServer(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugImGuiNetworking",
                "KLDebugImGuiUser",
                "KLDebugNetworkingRuntime",
                "KLDebugUtils",

                //engine
                "DeveloperSettings",
                "Engine",
                "Networking",
                "Sockets",
                "StructUtils"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
            }
        );
    }
}
