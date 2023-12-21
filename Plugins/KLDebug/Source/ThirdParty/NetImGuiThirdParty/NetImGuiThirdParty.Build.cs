using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class NetImGuiThirdParty : ModuleRules
{
    public NetImGuiThirdParty(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.NoSharedPCHs; // Prevents problem with Dear NetImgui sources not including the right first header
        PrivatePCHHeaderFile = "Public/NetImGuiThirdPartyModule.h";

        //---------------------------------------------------------------------
        // Settings configuration 
        //---------------------------------------------------------------------
        bool kUseNetImgui			= true;									// Toggle netImgui enabled here
		string kGameListenPort		= "(NetImgui::kDefaultClientPort)";		// Com Port used by Game exe to wait for a connection from netImgui Server (8889 by default)
		string kEditorListenPort	= "(NetImgui::kDefaultClientPort+1)";   // Com Port used by Editor exe to wait for a connection from netImgui Server (8890 by default)
		string kServerListenPort	= "(NetImgui::kDefaultClientPort+2)";   // Com Port used by Server exe to wait for a connection from netImgui Server (8891 by default)

		//---------------------------------------------------------------------
		// Setup Environment to build with/without netImgui
		//---------------------------------------------------------------------
		//Type = ModuleType.External;

		if( kUseNetImgui )
		{
            string PrivateFolder = Path.Combine(ModuleDirectory, "Private");
            string PublicLibraryFolder = Path.Combine(ModuleDirectory, "Public/Library");
            string PublicImGuiLibraryFolder = Path.Combine(ModuleDirectory, "../ImGuiThirdParty/Public/Library");

            PrivateIncludePaths.AddRange(
                new string[] {
                    PublicImGuiLibraryFolder,
                    PublicLibraryFolder,
                    PrivateFolder
                }
            );

            PublicDependencyModuleNames.AddRange(
               new string[]
               {
                   "ImGuiThirdParty",

                   //engine
                   "Core",
                   "Sockets"
                   // ... add other public dependencies that you statically link with here ...
               }
           );

			PublicDefinitions.Add("NETIMGUI_ENABLED=1");
			PublicDefinitions.Add("NETIMGUI_LISTENPORT_GAME=" + kGameListenPort);
			PublicDefinitions.Add("NETIMGUI_LISTENPORT_EDITOR=" + kEditorListenPort);
			PublicDefinitions.Add("NETIMGUI_LISTENPORT_DEDICATED_SERVER=" + kServerListenPort);
			PublicDefinitions.Add("NETIMGUI_IMGUI_CALLBACK_ENABLED=0");	// Disabled NetImgui intercepting the NewFrame/Render of Imgui Contexts
			PublicDefinitions.Add("NETIMGUI_WINSOCKET_ENABLED=0");      // Using Unreal sockets, no need for built-in sockets
			PublicDefinitions.Add("NETIMGUI_POSIX_SOCKETS_ENABLED=0");  // Using Unreal sockets, no need for built-in sockets
            PublicDefinitions.Add("IMGUI_UNREAL_COMMAND_ENABLED=1");  // Enable Unreal Command
            PublicDefinitions.Add("NETIMGUI_DEMO_ACTOR_ENABLED=1");  // Enable demo example
        }
		else
		{
			PublicDefinitions.Add("NETIMGUI_ENABLED=0");
		}
    }
}
