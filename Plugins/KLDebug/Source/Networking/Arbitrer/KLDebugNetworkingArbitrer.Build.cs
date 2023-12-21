using System.IO;
using UnrealBuildTool;

public class KLDebugNetworkingArbitrer : ModuleRules
{
    public KLDebugNetworkingArbitrer(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugNetworkingRuntime",

                //engine
                "DeveloperSettings",
                "Networking",
                "Projects",
                "Sockets"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject" //needed for DeveloperSettings
            }
        );
    }
}
