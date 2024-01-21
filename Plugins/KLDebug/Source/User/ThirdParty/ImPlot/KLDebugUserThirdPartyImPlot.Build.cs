using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugUserThirdPartyImPlot : ModuleRules
{
    public KLDebugUserThirdPartyImPlot(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.NoSharedPCHs; // Prevents problem with Dear ImGui/NetImgui sources not including the right first header
        PrivatePCHHeaderFile = "Public/KLDebugUserThirdPartyImPlotModule.h";

        string PrivateFolder = Path.Combine(ModuleDirectory, "Private");
        string PublicLibraryFolder = Path.Combine(ModuleDirectory, "Public/Library");
        string PublicImGuiLibraryFolder = Path.Combine(ModuleDirectory, "../ImGui/Public/Library");

        PrivateIncludePaths.AddRange(
            new string[] {
                PublicImGuiLibraryFolder,
                PublicLibraryFolder,
                PrivateFolder,
            }
        );

        PublicDependencyModuleNames.AddRange(
           new string[]
           {
                "KLDebugUserThirdPartyImGui",

                "Core"
               // ... add other public dependencies that you statically link with here ...
           }
       );

        PublicDefinitions.Add("IMGUI_DEFINE_MATH_OPERATORS");
    }
}
