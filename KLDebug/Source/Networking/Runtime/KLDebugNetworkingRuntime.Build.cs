using System.IO;
using UnrealBuildTool;

public class KLDebugNetworkingRuntime : ModuleRules
{
    public KLDebugNetworkingRuntime(ReadOnlyTargetRules Target) : base(Target)
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
