using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugEditorViewport : ModuleRules
{
    public KLDebugEditorViewport(ReadOnlyTargetRules Target) : base(Target)
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
                "Engine"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "LevelEditor",
                "Slate",
                "SlateCore",
                "UnrealEd"
            }
         );
    }
}
