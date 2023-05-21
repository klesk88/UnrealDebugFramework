using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class ImGuiNodeEditor : ModuleRules
{
    public ImGuiNodeEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.NoSharedPCHs; // Prevents problem with Dear ImGui sources not including the right first header
        PrivatePCHHeaderFile = "Public/ImGuiNodeEditorModule.h";

        string PrivateFolder = Path.Combine(ModuleDirectory, "Private");
        string PublicLibraryFolder = Path.Combine(ModuleDirectory, "Public/Library");
        string PublicImGuiLibraryFolder = Path.Combine(ModuleDirectory, "../ImGuiThirdParty/Public/Library");

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
               "Core",
               "ImGuiThirdParty"
               // ... add other public dependencies that you statically link with here ...
           }
       );
    }
}
