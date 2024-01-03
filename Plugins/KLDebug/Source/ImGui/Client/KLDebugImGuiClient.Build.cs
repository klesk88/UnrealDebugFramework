using System.IO;
using UnrealBuildTool;

public class KLDebugImGuiClient : ModuleRules
{
    public KLDebugImGuiClient(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugUserNetworking",
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
