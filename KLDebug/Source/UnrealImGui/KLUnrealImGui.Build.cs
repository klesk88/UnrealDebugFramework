using System.IO;
using UnrealBuildTool;

public class KLUnrealImGui : ModuleRules
{
    public KLUnrealImGui(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
          new string[]
          {
                Path.Combine(ModuleDirectory, "Private"),
                Path.Combine(ModuleDirectory, "../../../UnrealImGui/Source/UnrealImGui/Private"),
                Path.Combine(ModuleDirectory, "../../../UnrealImGui/Source/UnrealImGui/Private/Editor"),
                Path.Combine(ModuleDirectory, "../../../UnrealImGui/Source/UnrealImGui/Private/Utilities"),
                Path.Combine(ModuleDirectory, "../../../UnrealImGui/Source/UnrealImGui/Private/Widgets")
          }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "ImGuiThirdParty",
                "KLDebugUtils",
                "NetImGuiThirdParty",
                "UnrealImGui",
				
                //engine
                "AIModule",
                "Engine",
                "GameplayTags",
                "StructUtils",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealImGui",

                //engine
                "Core",
                "CoreUObject"
            }
        );

        //Font support

        // Will load Japanese font 
        // Note: If not using Japanese, set this to false, saves on memory (avoids 6MB font data table source include)
        PublicDefinitions.Add("NETIMGUI_FONT_JAPANESE=0");
        // Will load the 'Kenney Game Icons' font Gaming oriented icons
        PublicDefinitions.Add("NETIMGUI_FONT_ICON_GAMEKENNEY=1");
        // Will load the 'FontAwesome 6' font ('free' subset) Contains various icons for every use
        PublicDefinitions.Add("NETIMGUI_FONT_ICON_AWESOME=1");
        // Will load 'Google Material Designs icons' font Contains various icons for every use
        PublicDefinitions.Add("NETIMGUI_FONT_ICON_MATERIALDESIGN=0");

        //Font Support
    }
}
