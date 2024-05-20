using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugUserThirdPartyImGui : ModuleRules
{
    public KLDebugUserThirdPartyImGui(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.NoSharedPCHs; // Prevents problem with Dear ImGui sources not including the right first header
        PrivatePCHHeaderFile = "Public/KLDebugUserThirdPartyImGuiModule.h";
        bUseUnity = false;

        string PrivateFolder = Path.Combine(ModuleDirectory, "Private");
        string PrivateImguiFolder = Path.Combine(ModuleDirectory, "Private/Library");
        string PublicImguiFolder = Path.Combine(ModuleDirectory, "Public/Library");

        PrivateIncludePaths.AddRange(
            new string[] {
                PublicImguiFolder,
                PrivateImguiFolder,
                PrivateFolder,
            }
        );

        PublicDependencyModuleNames.AddRange(
           new string[]
           {
               "Core"
               // ... add other public dependencies that you statically link with here ...
           }
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string PublicImguiFreeTypeFolder = Path.Combine(ModuleDirectory, "Public/Library/misc/freetype");

            PrivateIncludePaths.AddRange(
                new string[] {
                    PublicImguiFreeTypeFolder,
                }
            );

            AddEngineThirdPartyPrivateStaticDependencies(Target, "FreeType2");

            PublicDefinitions.Add("NETIMGUI_FREETYPE_ENABLED=1");
        }
        else
        {
            //disable for linux builds
            PublicDefinitions.Add("NETIMGUI_FREETYPE_ENABLED=0");
        }

        //Begin KLMod add this define here otherwise i receive warnings
        PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS");
    }
}
