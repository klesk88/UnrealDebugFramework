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
                "KLDebugNetworkingArbitrer",
                "KLDebugNetworkingRuntime",
                "KLDebugUtils",

                //engine
                "Engine",
                "Networking",
                "Sockets"
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
