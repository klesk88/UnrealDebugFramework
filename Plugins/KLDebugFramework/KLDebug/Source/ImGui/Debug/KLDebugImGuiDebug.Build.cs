using System.IO;
using UnrealBuildTool;

public class KLDebugImGuiDebug : ModuleRules
{
    public KLDebugImGuiDebug(ReadOnlyTargetRules Target) : base(Target)
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
                "ImGuiThirdParty",
                "KLDebugImGuiClient",
                "KLDebugImGuiFramework",
                "KLDebugImGuiNetworking",
                "KLDebugImGuiServer",
                "KLDebugImGuiUser",
                
                //engine
                "Engine",
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
