using System.IO;
using UnrealBuildTool;

public class KLDebugUserNetworking : ModuleRules
{
    public KLDebugUserNetworking(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
          new string[]
          {
                Path.Combine(ModuleDirectory)
          }
        );

        PrivateIncludePaths.AddRange(
          new string[]
          {
                Path.Combine(ModuleDirectory, "Private")
          }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "KLDebugUtils"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core"
            }
        );
    }
}
