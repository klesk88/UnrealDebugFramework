using System.IO;
using UnrealBuildTool;

public class KLDebugUserFramework : ModuleRules
{
    public KLDebugUserFramework(ReadOnlyTargetRules Target) : base(Target)
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
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject"
            }
        );
    }
}
