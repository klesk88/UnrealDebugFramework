using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugGameplayRuntime : ModuleRules
{
    public KLDebugGameplayRuntime(ReadOnlyTargetRules Target) : base(Target)
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
                "KLDebugImGuiFramework",
                "KLDebugUserGameplay",
                "KLDebugUtils",

                //engine
                "ApplicationCore",
                "DeveloperSettings",
                "Engine",
                "EnhancedInput",
                "InputCore",
                "Slate",
                "StructUtils"
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject"
            }
         );
    }
}
