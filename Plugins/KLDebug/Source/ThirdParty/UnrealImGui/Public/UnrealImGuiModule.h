// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ImGuiDelegates.h"
#include "ImGuiModuleProperties.h"
#include "ImGuiTextureHandle.h"

#include <Modules/ModuleManager.h>

//@Begin KLMod: forward declarations
class FImGuiModuleManager;
//@End KLMod

//@Begin KLMod: added API
class UNREALIMGUI_API FUnrealImGuiModule : public IModuleInterface
{
public:
    //@Begin KLMod : disable all this I have my own

    //enum class eFont
    //{
    //	kProggyClean,
    //	kCousineRegular,
    //	kDroidSans,
    //	kKarlaRegular,
    //	kProggyTiny,
    //	kRobotoMedium,

    //	//... Your own font can be added here and loaded in 'FImGuiContextManager::BuildFontAtlas()'

    //	_Count,
    //};

    //@End KLMod

    /**
     * Singleton-like access to this module's interface. This is just for convenience!
     * Beware of calling this during the shutdown phase, though. Your module might have been unloaded already.
     *
     * @return Returns singleton instance, loading the module on demand if needed
     */
    static inline FUnrealImGuiModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FUnrealImGuiModule>("UnrealImGui");
    }

    /**
     * Checks to see if this module is loaded and ready. It is only valid to call Get() if IsAvailable() returns true.
     *
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("UnrealImGui");
    }

#if IMGUI_WITH_OBSOLETE_DELEGATES

#if WITH_EDITOR
    /**
     * Add a delegate called at the end of editor debug frame to draw debug controls in its ImGui context, creating
     * that context on demand.
     *
     * @param Delegate - Delegate that we want to add (@see FImGuiDelegate::Create...)
     * @returns Returns handle that can be used to remove delegate (@see RemoveImGuiDelegate)
     */
    virtual FImGuiDelegateHandle AddEditorImGuiDelegate(const FImGuiDelegate& Delegate);
#endif

    /**
     * Add a delegate called at the end of current world debug frame to draw debug controls in its ImGui context,
     * creating that context on demand.
     * This function will throw if called outside of a world context (i.e. current world cannot be found).
     *
     * @param Delegate - Delegate that we want to add (@see FImGuiDelegate::Create...)
     * @returns Returns handle that can be used to remove delegate (@see RemoveImGuiDelegate)
     */
    virtual FImGuiDelegateHandle AddWorldImGuiDelegate(const FImGuiDelegate& Delegate);

    /**
     * Add a delegate called at the end of a specific world's debug frame to draw debug controls in its ImGui context,
     * creating that context on demand.
     *
     * @param World - A specific world to add the delegate to to
     * @param Delegate - Delegate that we want to add (@see FImGuiDelegate::Create...)
     * @returns Returns handle that can be used to remove delegate (@see RemoveImGuiDelegate)
     */
    virtual FImGuiDelegateHandle AddWorldImGuiDelegate(const UWorld* World, const FImGuiDelegate& Delegate);

    /**
     * Add shared delegate called for each ImGui context at the end of debug frame, after calling context specific
     * delegate. This delegate will be used for any ImGui context, created before or after it is registered.
     *
     * @param Delegate - Delegate that we want to add (@see FImGuiDelegate::Create...)
     * @returns Returns handle that can be used to remove delegate (@see RemoveImGuiDelegate)
     */
    virtual FImGuiDelegateHandle AddMultiContextImGuiDelegate(const FImGuiDelegate& Delegate);

    /**
     * Remove delegate added with any version of Add...ImGuiDelegate
     *
     * @param Handle - Delegate handle that was returned by adding function
     */
    virtual void RemoveImGuiDelegate(const FImGuiDelegateHandle& Handle);

#endif // #if IMGUI_WITH_OBSOLETE_DELEGATES

    /**
     * If it exists, get a handle to the texture with given resource name.
     *
     * @param Name - Resource name of a texture to find
     * @returns Handle to a registered texture or invalid handle if resources could not be found or were not valid
     */
    virtual FImGuiTextureHandle FindTextureHandle(const FName& Name);

    /**
     * Register texture and create its Slate resources. If texture with that name already exists then it may be updated
     * or if bMakeUnique is true, exception will be thrown. Throws exception, if name argument is NAME_None or texture
     * is null.
     *
     * Note, that updating texture resources doesn't invalidate already existing handles and returned handle will have
     * the same value.
     *
     * @param Name - Resource name for the texture that needs to be registered or updated
     * @param Texture - Texture for which we want to create or update Slate resources
     * @param bMakeUnique - If false then existing resources are updated/overwritten (default). If true, then stricter
     *     policy is applied and if resource with that name exists then exception is thrown.
     * @returns Handle to the texture resources, which can be used to release allocated resources and as an argument to
     *     relevant ImGui functions
     */
    virtual FImGuiTextureHandle RegisterTexture(const FName& Name, class UTexture* Texture, bool bMakeUnique = false);

    /**
     * Unregister texture and release its Slate resources. If handle is null or not valid, this function fails silently
     * (for definition of 'valid' look @ FImGuiTextureHandle).
     *
     * @returns ImGui Texture Handle to texture that needs to be unregistered
     */
    virtual void ReleaseTexture(const FImGuiTextureHandle& Handle);

    virtual void RebuildFontAtlas();

    /**
     * Get ImGui module properties.
     *
     * @returns Reference to an instance of ImGui module properties that allows to read and/or modify module state.
     */
    virtual FImGuiModuleProperties& GetProperties();
    virtual const FImGuiModuleProperties& GetProperties() const;

    /** Check whether we are ~currently~ drawing for remote connection or locally **/
    virtual bool IsRemoteDrawing() const;

    /** Check whether we have a remote connection established or not **/
    virtual bool IsRemoteConnected() const;

    //@Begin KLMod : disable all these methods as I have my own

    /** Change the current Font used for drawing (matching PopFont() is expected). Can also use struct ImGuiScopedFont **/
    //static inline void PushFont(eFont font)
    //{
    //	check(font < eFont::_Count);
    //	ImFont* pFont = font < eFont::_Count ? ImGui::GetIO().Fonts->Fonts[static_cast<int>(font)] : nullptr;
    //	ImGui::PushFont(pFont ? pFont : ImGui::GetFont());
    //}

    ///**
    // * Undo the last PushFont(), returning it to previous value. 
    // */
    //static inline void PopFont()
    //{
    //	ImGui::PopFont();
    //}

    //@End KLMod

    /**
     * DEPRECIATED: Please use GetProperties() as this function is scheduled for removal.
     * Check whether Input Mode is enabled (tests ImGui.InputEnabled console variable).
     *
     * @returns True, if Input Mode is enabled (ImGui.InputEnabled != 0) and false otherwise.
     */
    virtual bool IsInputMode() const;

    /**
     * DEPRECIATED: Please use GetProperties() as this function is scheduled for removal.
     * Set Input Mode state (sets ImGui.InputEnabled console variable, so it can be used together with a console).
     *
     * @param bEnabled - Whether Input Mode should be enabled (ImGui.InputEnabled = 1) or not (ImGui.InputEnabled = 0).
     */
    virtual void SetInputMode(bool bEnabled);

    /**
     * DEPRECIATED: Please use GetProperties() as this function is scheduled for removal.
     * Toggle Input Mode state (changes ImGui.InputEnabled console variable).
     */
    virtual void ToggleInputMode();

    /**
     * DEPRECIATED: Please use GetProperties() as this function is scheduled for removal.
     * Check whether ImGui Demo is shown (tests ImGui.ShowDemo console variable).
     *
     * @returns True, if demo is shown (ImGui.ShowDemo != 0) and false otherwise.
     */
    virtual bool IsShowingDemo() const;

    /**
     * DEPRECIATED: Please use GetProperties() as this function is scheduled for removal.
     * Set whether to show ImGui Demo (sets ImGui.ShowDemo console variable, so it can be used together with a console).
     *
     * @param bShow - Whether to show ImGui Demo (ImGui.ShowDemo = 1) or not (ImGui.ShowDemo = 0).
     */
    virtual void SetShowDemo(bool bShow);

    /**
     * DEPRECIATED: Please use GetProperties() as this function is scheduled for removal.
     * Toggle ImGui Demo (changes ImGui.ShowDemo console variable).
     */
    virtual void ToggleShowDemo();

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:

#if WITH_EDITOR
    virtual void SetProperties(const FImGuiModuleProperties& Properties);
    struct FImGuiContextHandle* ImGuiContextHandle = nullptr;
    struct FImGuiDelegatesContainerHandle* DelegatesContainerHandle = nullptr;
    friend struct FImGuiContextHandle;
    friend struct FImGuiDelegatesContainerHandle;
#endif

    //@Begin KLMod: methods that we want to ovverride from child
protected:
    virtual void Init();
    virtual void Shutdown();

    [[nodiscard]] FImGuiModuleManager& GetImguiModuleManager() const;
    //@End KLMod
};

//@Begin KLMod : disable all this I have my own

/**
// Helper class that change the font and automatically restore it when object is out of scope
*/
//struct ImGuiScopedFont
//{
//	inline ImGuiScopedFont(FUnrealImGuiModule::eFont font){ FUnrealImGuiModule::PushFont(font); }
//	inline ~ImGuiScopedFont(){ FUnrealImGuiModule::PopFont(); }
//};

//@End KLMod