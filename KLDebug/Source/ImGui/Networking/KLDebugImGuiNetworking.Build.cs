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
                "KLDebugImGuiUser",
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
