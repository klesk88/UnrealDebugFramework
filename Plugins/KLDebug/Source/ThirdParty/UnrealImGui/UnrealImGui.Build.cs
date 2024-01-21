// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

//Begin @KLMod change module name to dont have porlbmes with base imgui
public class UnrealImGui : ModuleRules
{
#if WITH_FORWARDED_MODULE_RULES_CTOR
    public UnrealImGui(ReadOnlyTargetRules Target) : base(Target)
#else
	public UnrealImGui(TargetInfo Target)
#endif
    {

#if WITH_FORWARDED_MODULE_RULES_CTOR
        bool bBuildEditor = Target.bBuildEditor;
#else
		bool bBuildEditor = (Target.Type == TargetRules.TargetType.Editor);
#endif

        // Developer modules are automatically loaded only in editor builds but can be stripped out from other builds.
        // Enable runtime loader, if you want this module to be automatically loaded in runtime builds (monolithic).
        bool bEnableRuntimeLoader = false;
        bool bUnrealImguiEnabled = true;
        //Begin @KLMod removed this not needed
        //bool bEnableImUnrealCommand = true;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

#if UE_4_24_OR_LATER
        bLegacyPublicIncludePaths = false;
        ShadowVariableWarningLevel = WarningLevel.Error;
        bTreatAsEngineModule = true;
#endif

        PublicIncludePaths.AddRange(
            new string[] {
				//Begin @KLMod fix includes
                Path.Combine(PluginDirectory, "Source/User/ThirdParty/ImGui/Public/Library"),
                Path.Combine(PluginDirectory, "Source/User/ThirdParty/ImPlot/Public/Library"),
				//End @KLMod fix includes
                //Path.Combine(ModuleDirectory, "../ThirdParty/ImGuiLibrary/Include"),
                //Path.Combine(ModuleDirectory, "../ThirdParty/ImPlotLibrary/Public")
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                Path.Combine(ModuleDirectory, "Private"),
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Projects"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "InputCore",
                "Slate",
                "SlateCore",

				//KLMod: add correct modules
				"KLDebugUserThirdPartyImGui",
                "KLDebugUserThirdPartyImPlot",
                "NetImGuiThirdParty"
            }
            );


        if (bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "EditorStyle",
                    "Settings",
                    "UnrealEd",
                }
                );
        }


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );


#if !UE_4_19_OR_LATER
		List<string> PrivateDefinitions = Definitions;
#endif
        PublicDefinitions.Add(string.Format("UNREAL_IMGUI_ENABLED={0}", bUnrealImguiEnabled ? 1 : 0));
        PrivateDefinitions.Add(string.Format("RUNTIME_LOADER_ENABLED={0}", bEnableRuntimeLoader ? 1 : 0));
    }
}
