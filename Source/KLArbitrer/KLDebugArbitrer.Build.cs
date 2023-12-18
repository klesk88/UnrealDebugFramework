using System.IO;
using UnrealBuildTool;

public class KLDebugArbitrer : ModuleRules
{
    public KLDebugArbitrer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
          new string[]
          {
                Path.Combine(ModuleDirectory, "Private"),
          }
        );

        PublicIncludePathModuleNames.Add("Launch");

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "KLDebugNetworkingArbitrer",
                "KLDebugNetworkingRuntime",

                //engine
                "ApplicationCore",
                "Networking",
                "TargetPlatform",
                "Sockets"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Projects"
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[] {
                    "UnixCommonStartup"
                }
            );
        }
    }
}
