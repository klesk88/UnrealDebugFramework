// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "UnrealImGuiModule.h"

#include "ImGuiDelegatesContainer.h"
#include "ImGuiModuleManager.h"
#include "TextureManager.h"
#include "Utilities/WorldContext.h"
#include "Utilities/WorldContextIndex.h"
#include "ThirdPartyBuildNetImgui.h"

#if WITH_EDITOR
#include "ThirdPartyBuildImGui.h"
#include "Editor/ImGuiEditor.h"
#endif

#include <Interfaces/IPluginManager.h>


#define LOCTEXT_NAMESPACE "FUnrealImGuiModule"


struct EDelegateCategory
{
	enum
	{
		// Default per-context draw events.
		Default,

		// Multi-context draw event defined in context manager.
		MultiContext
	};
};

FImGuiModuleManager* ImGuiModuleManager = nullptr;

#if WITH_EDITOR
static FImGuiEditor* ImGuiEditor = nullptr;
#endif

#if IMGUI_WITH_OBSOLETE_DELEGATES

#if WITH_EDITOR
FImGuiDelegateHandle FUnrealImGuiModule::AddEditorImGuiDelegate(const FImGuiDelegate& Delegate)
{
	return { FImGuiDelegatesContainer::Get().OnWorldDebug(Utilities::EDITOR_CONTEXT_INDEX).Add(Delegate),
		EDelegateCategory::Default, Utilities::EDITOR_CONTEXT_INDEX };
}
#endif

FImGuiDelegateHandle FUnrealImGuiModule::AddWorldImGuiDelegate(const FImGuiDelegate& Delegate)
{
	const int32 ContextIndex = Utilities::GetWorldContextIndex((UWorld*)GWorld);
	return { FImGuiDelegatesContainer::Get().OnWorldDebug(ContextIndex).Add(Delegate), EDelegateCategory::Default, ContextIndex };
}

FImGuiDelegateHandle FUnrealImGuiModule::AddWorldImGuiDelegate(const UWorld* World, const FImGuiDelegate& Delegate)
{
	const int32 ContextIndex = Utilities::GetWorldContextIndex(World);
	return { FImGuiDelegatesContainer::Get().OnWorldDebug(ContextIndex).Add(Delegate), EDelegateCategory::Default, ContextIndex };
}

FImGuiDelegateHandle FUnrealImGuiModule::AddMultiContextImGuiDelegate(const FImGuiDelegate& Delegate)
{
	return { FImGuiDelegatesContainer::Get().OnMultiContextDebug().Add(Delegate), EDelegateCategory::MultiContext };
}

void FUnrealImGuiModule::RemoveImGuiDelegate(const FImGuiDelegateHandle& Handle)
{
	if (Handle.Category == EDelegateCategory::MultiContext)
	{
		FImGuiDelegatesContainer::Get().OnMultiContextDebug().Remove(Handle.Handle);
	}
	else
	{
		FImGuiDelegatesContainer::Get().OnWorldDebug(Handle.Index).Remove(Handle.Handle);
	}
}

#endif // IMGUI_WITH_OBSOLETE_DELEGATES

FImGuiTextureHandle FUnrealImGuiModule::FindTextureHandle(const FName& Name)
{
	const TextureIndex Index = ImGuiModuleManager->GetTextureManager().FindTextureIndex(Name);
	return (Index != INDEX_NONE) ? FImGuiTextureHandle{ Name, ImGuiInterops::ToImTextureID(Index) } : FImGuiTextureHandle{};
}

FImGuiTextureHandle FUnrealImGuiModule::RegisterTexture(const FName& Name, class UTexture* Texture, bool bMakeUnique)
{
	FTextureManager& TextureManager = ImGuiModuleManager->GetTextureManager();

	checkf(!bMakeUnique || TextureManager.FindTextureIndex(Name) == INDEX_NONE,
		TEXT("Trying to register a texture with a name '%s' that is already used. Chose a different name ")
		TEXT("or use bMakeUnique false, to update existing texture resources."), *Name.ToString());

	const TextureIndex Index = TextureManager.CreateTextureResources(Name, Texture);
	return FImGuiTextureHandle{ Name, ImGuiInterops::ToImTextureID(Index) };
}

void FUnrealImGuiModule::ReleaseTexture(const FImGuiTextureHandle& Handle)
{
	if (Handle.IsValid())
	{
		ImGuiModuleManager->GetTextureManager().ReleaseTextureResources(ImGuiInterops::ToTextureIndex(Handle.GetTextureId()));
	}
}

void FUnrealImGuiModule::RebuildFontAtlas()
{
	if (ImGuiModuleManager)
	{
		ImGuiModuleManager->RebuildFontAtlas();
	}
}

void FUnrealImGuiModule::StartupModule()
{
    //@Begin KLMod: disable all initialization here as we will initialize teh module from FKLUnrealImGuiModule
    // if we leave the code here we will call it once when unreal loads this module and another time when we load mine
    // Init();
    // add this method so we can override it from the child
    // ImGuiModuleManager->GetContextManager().Init();
    //@End KLMod
}

void FUnrealImGuiModule::ShutdownModule()
{
    //@Begin KLMod: disable shutdown. it will be handle from my module
    // Shutdown();
    //@End KLMod
}

#if WITH_EDITOR
void FUnrealImGuiModule::SetProperties(const FImGuiModuleProperties& Properties)
{
	ImGuiModuleManager->GetProperties() = Properties;
}
#endif

FImGuiModuleProperties& FUnrealImGuiModule::GetProperties()
{
	return ImGuiModuleManager->GetProperties();
}

const FImGuiModuleProperties& FUnrealImGuiModule::GetProperties() const
{
	return ImGuiModuleManager->GetProperties();
}

bool FUnrealImGuiModule::IsInputMode() const
{
	return ImGuiModuleManager && ImGuiModuleManager->GetProperties().IsInputEnabled();
}

void FUnrealImGuiModule::SetInputMode(bool bEnabled)
{
	if (ImGuiModuleManager)
	{
		ImGuiModuleManager->GetProperties().SetInputEnabled(bEnabled);
	}
}

void FUnrealImGuiModule::ToggleInputMode()
{
	if (ImGuiModuleManager)
	{
		ImGuiModuleManager->GetProperties().ToggleInput();
	}
}

bool FUnrealImGuiModule::IsShowingDemo() const
{
	return ImGuiModuleManager && ImGuiModuleManager->GetProperties().ShowDemo();
}

void FUnrealImGuiModule::SetShowDemo(bool bShow)
{
	if (ImGuiModuleManager)
	{
		ImGuiModuleManager->GetProperties().SetShowDemo(bShow);
	}
}

void FUnrealImGuiModule::ToggleShowDemo()
{
	if (ImGuiModuleManager)
	{
		ImGuiModuleManager->GetProperties().ToggleDemo();
	}
}

bool FUnrealImGuiModule::IsRemoteDrawing() const
{
#if NETIMGUI_ENABLED
	return NetImgui::IsDrawingRemote();
#else
	return false;
#endif
}

bool FUnrealImGuiModule::IsRemoteConnected() const
{
#if NETIMGUI_ENABLED
	return NetImgui::IsConnected();
#else
	return false;
#endif
}

//----------------------------------------------------------------------------------------------------
// Runtime loader
//----------------------------------------------------------------------------------------------------

#if !WITH_EDITOR && RUNTIME_LOADER_ENABLED

class FImGuiModuleLoader
{
	FImGuiModuleLoader()
	{
		if (!Load())
		{
			FModuleManager::Get().OnModulesChanged().AddRaw(this, &FImGuiModuleLoader::LoadAndRelease);
		}
	}

	// For different engine versions.
	static FORCEINLINE bool IsValid(const TSharedPtr<IModuleInterface>& Ptr) { return Ptr.IsValid(); }
	static FORCEINLINE bool IsValid(const IModuleInterface* const Ptr) { return Ptr != nullptr; }

	bool Load()
	{
		return IsValid(FModuleManager::Get().LoadModule(ModuleName));
	}

	void LoadAndRelease(FName Name, EModuleChangeReason Reason)
	{
		// Avoid handling own load event.
		if (Name != ModuleName)
		{
			// Try loading until success and then release.
			if (Load())
			{
				FModuleManager::Get().OnModulesChanged().RemoveAll(this);
			}
		}
	}

	static FName ModuleName;

	static FImGuiModuleLoader Instance;
};

FName FImGuiModuleLoader::ModuleName = "ImGui";

// In monolithic builds this will start loading process.
FImGuiModuleLoader FImGuiModuleLoader::Instance;

#endif // !WITH_EDITOR && RUNTIME_LOADER_ENABLED


//----------------------------------------------------------------------------------------------------
// Partial implementations of other classes that needs access to ImGuiModuleManager
//----------------------------------------------------------------------------------------------------

bool FImGuiTextureHandle::HasValidEntry() const
{
	const TextureIndex Index = ImGuiInterops::ToTextureIndex(TextureId);
	return Index != INDEX_NONE && ImGuiModuleManager && ImGuiModuleManager->GetTextureManager().GetTextureName(Index) == Name;
}

//@Begin KLMod

// allow to override Init method called from when the module is startup
void FUnrealImGuiModule::Init()
{
    // Initialize handles to allow cross-module redirections. Other handles will always look for parents in the active
    // module, which means that we can only redirect to started modules. We don't have to worry about self-referencing
    // as local handles are guaranteed to be constructed before initializing pointers.
    // This supports in-editor recompilation and hot-reloading after compiling from the command line. The latter method
    // theoretically doesn't support plug-ins and will not load re-compiled module, but its handles will still redirect
    // to the active one.

#if WITH_EDITOR
    ImGuiContextHandle       = &ImGuiImplementation::GetContextHandle();
    DelegatesContainerHandle = &FImGuiDelegatesContainer::GetHandle();
#endif

    // Create managers that implements module logic.

    checkf(!ImGuiModuleManager, TEXT("Instance of the ImGui Module Manager already exists. Instance should be created only during module startup."));
    ImGuiModuleManager = new FImGuiModuleManager();

#if WITH_EDITOR
    checkf(!ImGuiEditor, TEXT("Instance of the ImGui Editor already exists. Instance should be created only during module startup."));
    ImGuiEditor = new FImGuiEditor();
#endif

    NetImGuiStartup();
}

// allow to override shutdown method called from when the module is shutting down
void FUnrealImGuiModule::Shutdown()
{
    // In editor store data that we want to move to hot-reloaded module.
#if WITH_EDITOR
    static bool                   bMoveProperties  = true;
    static FImGuiModuleProperties PropertiesToMove = ImGuiModuleManager->GetProperties();
#endif

    // Before we shutdown we need to delete managers that will do all the necessary cleanup.

#if WITH_EDITOR
    checkf(ImGuiEditor, TEXT("Null ImGui Editor. ImGui editor instance should be deleted during module shutdown."));
    delete ImGuiEditor;
    ImGuiEditor = nullptr;
#endif

    checkf(ImGuiModuleManager, TEXT("Null ImGui Module Manager. Module manager instance should be deleted during module shutdown."));
    delete ImGuiModuleManager;
    ImGuiModuleManager = nullptr;

    NetImGuiShutdown();

#if WITH_EDITOR
    // When shutting down we leave the global ImGui context pointer and handle pointing to resources that are already
    // deleted. This can cause troubles after hot-reload when code in other modules calls ImGui interface functions
    // which are statically bound to the obsolete module. To keep ImGui code functional we can redirect context handle
    // to point to the new module.

    // When shutting down during hot-reloading, we might want to rewire handles used in statically bound functions
    // or move data to a new module.

    FModuleManager::Get().OnModulesChanged().AddLambda([this](FName Name, EModuleChangeReason Reason)
                                                       {
		if (Reason == EModuleChangeReason::ModuleLoaded && Name == "ImGui")
		{
			FUnrealImGuiModule& LoadedModule = FUnrealImGuiModule::Get();
			if (&LoadedModule != this)
			{
				// Statically bound functions can be bound to the obsolete module, so we need to manually redirect.

				if (LoadedModule.ImGuiContextHandle)
				{
					ImGuiImplementation::SetParentContextHandle(*LoadedModule.ImGuiContextHandle);
				}

				if (LoadedModule.DelegatesContainerHandle)
				{
					FImGuiDelegatesContainer::MoveContainer(*LoadedModule.DelegatesContainerHandle);
				}

				if (bMoveProperties)
				{
					bMoveProperties = false;
					LoadedModule.SetProperties(PropertiesToMove);
				}
			}
		} });
#endif  // WITH_EDITOR
}

// allow to gran the module manager
FImGuiModuleManager& FUnrealImGuiModule::GetImguiModuleManager() const
{
    checkf(ImGuiModuleManager != nullptr, TEXT("not valid"));
    return *ImGuiModuleManager;
}

//@End KLMod


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealImGuiModule, ImGui)
