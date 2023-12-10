using UnrealBuildTool;
using System.Collections.Generic;

//base on ReplicationSystemTestTarget

[SupportedPlatforms("Win64", "Linux")]
public class KLDebugArbitrerTarget : TargetRules
{
    public KLDebugArbitrerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Program;
        LinkType = TargetLinkType.Monolithic;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        LaunchModuleName = "KLDebugArbitrer";
        DefaultBuildSettings = BuildSettingsVersion.Latest;

        // No editor-only data is needed
        bBuildWithEditorOnlyData = false;

        bCompileAgainstEngine = false;
        bCompileAgainstCoreUObject = true; //needed for DeveloperSettings
        bCompileAgainstApplicationCore = true;
        bCompileWithPluginSupport = true;
        bBuildDeveloperTools = false;
        bBuildRequiresCookedData = false;

        // No ICU internationalization.
        //bCompileICU = true;
        bCompileICU = false;
        bIncludePluginsForTargetPlatforms = false;

        // Don't need slate
        bUsesSlate = false;

        bIsBuildingConsoleApplication = true;
        bLegalToDistributeBinary = true;

        // Always want logging so we can see the test results
        bUseLoggingInShipping = true;

        // Network config
        bWithPushModel = true;
        bUseIris = true;
    }
}
