using System.IO;
using UnrealBuildTool;

public class KLDebugImGuiUser : ModuleRules
{
    public KLDebugImGuiUser(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
          new string[]
          {
                Path.Combine(ModuleDirectory, "Internal"),
                Path.Combine(ModuleDirectory, "Private"),
          }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "ImGuiThirdParty",
                "KLDebugUtils",

                //engine
                "Engine", //for FDebugDrawDelegateHelper
                "GameplayDebugger" //for FGameplayDebuggerCanvasContext
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
