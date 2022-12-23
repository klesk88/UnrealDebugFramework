using System.IO;
using UnrealBuildTool;

public class KLDebugImGui : ModuleRules
{
    public KLDebugImGui(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugCore",
				"KLDebugUtils",
                "KLUnrealImGui",
                "UnrealImGui",
				
                //engine
                "AIModule",
                "Engine",
                "GameplayTags",
                "InputCore",
                "StructUtils",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Slate",
                "SlateCore",


                "UnrealEd"
            }
        );
    }
}
