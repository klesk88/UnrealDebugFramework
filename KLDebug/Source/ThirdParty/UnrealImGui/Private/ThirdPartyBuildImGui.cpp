// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ThirdPartyBuildImGui.h"

#include <CoreMinimal.h>

// For convenience and easy access to the ImGui source code, we build it as part of this module.
// We don't need to define UNREALIMGUI_API manually because it is already done for this module.

//@Begin KLMod: use _XBOX_ONE
#ifdef _XBOX_ONE
// Disable Win32 functions used in ImGui and not supported on XBox.
#define IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
#define IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS
#endif // PLATFORM_XBOXONE

#if PLATFORM_WINDOWS
#include <Windows/AllowWindowsPlatformTypes.h>
#endif // PLATFORM_WINDOWS

#if WITH_EDITOR

#include "UnrealImGuiModule.h"
#include "Utilities/RedirectingHandle.h"

// Redirecting handle which will automatically bind to another one, if a different instance of the module is loaded.
struct FImGuiContextHandle : public Utilities::TRedirectingHandle<ImGuiContext*>
{
    FImGuiContextHandle(ImGuiContext*& InDefaultContext)
        : Utilities::TRedirectingHandle<ImGuiContext*>(InDefaultContext)
    {
        if (FUnrealImGuiModule* Module = FModuleManager::GetModulePtr<FUnrealImGuiModule>("UnrealImGui"))
        {
            SetParent(Module->ImGuiContextHandle);
        }
    }
};

static ImGuiContext* ImGuiContextPtr = nullptr;
static FImGuiContextHandle ImGuiContextPtrHandle(ImGuiContextPtr);

// Get the global ImGui context pointer (GImGui) indirectly to allow redirections in obsolete modules.
#define GImGui (ImGuiContextPtrHandle.Get())
#endif // WITH_EDITOR

//@Begin KLMod: disable thi .cpp not needed
//#include "imgui.cpp"
//#include "imgui_demo.cpp"
//#include "imgui_draw.cpp"
//#include "imgui_widgets.cpp"
//#include "imgui_tables.cpp"
//#include "implot.cpp"
//#include "implot_items.cpp"
//#include "implot_demo.cpp"
#if PLATFORM_WINDOWS
#include <Windows/HideWindowsPlatformTypes.h>
#endif // PLATFORM_WINDOWS

#include "ImGuiInteroperability.h"


namespace ImGuiImplementation
{
#if WITH_EDITOR
    FImGuiContextHandle& GetContextHandle()
    {
        return ImGuiContextPtrHandle;
    }

    void SetParentContextHandle(FImGuiContextHandle& Parent)
    {
        ImGuiContextPtrHandle.SetParent(&Parent);
    }
#endif // WITH_EDITOR
}
