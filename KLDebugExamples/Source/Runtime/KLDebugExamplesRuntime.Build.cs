using System.IO;
using UnrealBuildTool;

public class KLDebugExamplesRuntime : ModuleRules
{
    public KLDebugExamplesRuntime(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugCore",
				"KLDebugGameplay",
                "KLDebugImGui",

                //engine
                "Engine",
                "GameplayTags",
                "StructUtils",
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
