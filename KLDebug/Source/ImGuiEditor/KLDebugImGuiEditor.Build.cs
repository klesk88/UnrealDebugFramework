using System.IO;
using UnrealBuildTool;

public class KLDebugImGuiEditor : ModuleRules
{
    public KLDebugImGuiEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugImGui",
                "KLUnrealImGui",
                "UnrealImGui",
				
                //engine
                "DeveloperSettings",
                "Engine",
                "GameplayTags",
                "InputCore"
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
