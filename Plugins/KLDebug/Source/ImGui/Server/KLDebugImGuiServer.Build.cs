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
                Path.Combine(ModuleDirectory, "Private")
          }
        );

        RuntimeDependencies.Add(Path.Combine(PluginDirectory, "Resources/Arbitrer/KLDebugArbitrer.exe"));
        RuntimeDependencies.Add(Path.Combine(PluginDirectory, "Resources/Arbitrer/OpenColorIO_2_2.dll"));
        RuntimeDependencies.Add(Path.Combine(PluginDirectory, "Resources/Arbitrer/tbbmalloc.dll"));

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "KLDebugImGuiFramework",
                "KLDebugImGuiNetworking",
                "KLDebugNetworkingArbitrer",
                "KLDebugNetworkingRuntime",
                "KLDebugUserFramework",
                "KLDebugUtils",

                //engine
                "Engine",
                "Networking",
                "Sockets"
            }
        ); ;

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
            }
        );
    }
}
