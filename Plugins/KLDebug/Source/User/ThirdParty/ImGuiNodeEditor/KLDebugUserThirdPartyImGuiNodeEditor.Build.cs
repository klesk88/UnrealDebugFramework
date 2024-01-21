using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugUserThirdPartyImGuiNodeEditor : ModuleRules
{
    public KLDebugUserThirdPartyImGuiNodeEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.NoSharedPCHs; // Prevents problem with Dear ImGui sources not including the right first header
        PrivatePCHHeaderFile = "Public/KLDebugUserThirdPartyImGuiNodeEditorModule.h";

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
           }
       );
    }
}
