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

        RuntimeDependencies.Add(Path.Combine(PluginDirectory, "Resources/Arbitrer/KLDebugArbitrer.exe"));

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
                "Projects",
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
