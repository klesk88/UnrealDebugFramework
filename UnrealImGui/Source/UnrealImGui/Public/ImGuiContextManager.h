// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ImGuiContextProxy.h"
#include "VersionCompatibility.h"

// Begin KLMod
class UWorld;
// End KLMod

class FImGuiModuleSettings;
struct FImGuiDPIScaleInfo;

// TODO: It might be useful to broadcast FContextProxyCreatedDelegate to users, to support similar cases to our ImGui
// demo, but we would need to remove from that interface internal classes.

// Delegate called when new context proxy is created.
// @param ContextIndex - Index for that world
// @param ContextProxy - Created context proxy
DECLARE_MULTICAST_DELEGATE_TwoParams(FContextProxyCreatedDelegate, int32, FImGuiContextProxy&);

//@Begin KLMod: allow to overide font creation
DECLARE_DELEGATE_TwoParams(FOnBuildFontAtlas, float, ImFontAtlas&);
//@End KLMod

// NETIMGUI_ENABLED
// NOTE:	Made this public, to share with NetImgui. Need access to the proxy and CanTick property
//			Could be done other ways like building the data and sending it to NetImguiUpdate
//			instead of accessing it directly)
struct FContextData
{
    // Begin KLMod: Added world to constructor
    FContextData(const UWorld& _World, const FString& ContextName, int32 ContextIndex, ImFontAtlas& FontAtlas, float DPIScale, int32 InPIEInstance = -1)
        : PIEInstance(InPIEInstance)
        , ContextProxy(new FImGuiContextProxy(ContextName, ContextIndex, &FontAtlas, DPIScale, _World))
    {
    }

    FORCEINLINE bool CanTick() const
    {
        return PIEInstance < 0 || GEngine->GetWorldContextFromPIEInstance(PIEInstance);
    }

    // Begin KLMod update world contained by the proxy
    void UpdateProxyWorld(const UWorld& _World)
    {
        if (ContextProxy.IsValid())
        {
            ContextProxy->UpdateWorld(_World);
        }
    }
    // End KLMod

    int32                          PIEInstance = -1;
    TUniquePtr<FImGuiContextProxy> ContextProxy;
};
// NETIMGUI_ENABLED

// Manages ImGui context proxies.
//@Begin KLMod: moved to public folder
class UNREALIMGUI_API FImGuiContextManager
{
public:
    FImGuiContextManager(FImGuiModuleSettings& InSettings);

    FImGuiContextManager(const FImGuiContextManager&)            = delete;
    FImGuiContextManager& operator=(const FImGuiContextManager&) = delete;

    FImGuiContextManager(FImGuiContextManager&&)            = delete;
    FImGuiContextManager& operator=(FImGuiContextManager&&) = delete;

    ~FImGuiContextManager();

    ImFontAtlas& GetFontAtlas()
    {
        return FontAtlas;
    }
    const ImFontAtlas& GetFontAtlas() const
    {
        return FontAtlas;
    }

#if WITH_EDITOR
    // Get or create editor ImGui context proxy.
    // Begin KLMod: Removed unused function
    // FORCEINLINE FImGuiContextProxy& GetEditorContextProxy() { return *GetEditorContextData().ContextProxy; }
#endif

#if !WITH_EDITOR
    // Get or create standalone game ImGui context proxy.
    // Begin KLMod: Removed unused function
    // FORCEINLINE FImGuiContextProxy& GetWorldContextProxy() { return *GetStandaloneWorldContextData().ContextProxy; }
#endif

    // Get or create ImGui context proxy for given world.
    FORCEINLINE FImGuiContextProxy& GetWorldContextProxy(const UWorld& World)
    {
        return *GetWorldContextData(World).ContextProxy;
    }

    // Get or create ImGui context proxy for given world. Additionally get context index for that proxy.
    FORCEINLINE FImGuiContextProxy& GetWorldContextProxy(const UWorld& World, int32& OutContextIndex)
    {
        return *GetWorldContextData(World, &OutContextIndex).ContextProxy;
    }

    // Get context proxy by index, or null if context with that index doesn't exist.
    FORCEINLINE FImGuiContextProxy* GetContextProxy(int32 ContextIndex)
    {
        FContextData* Data = Contexts.Find(ContextIndex);
        return Data ? Data->ContextProxy.Get() : nullptr;
    }

    // Delegate called when a new context proxy is created.
    FContextProxyCreatedDelegate OnContextProxyCreated;

    // Delegate called after font atlas is built.
    FSimpleMulticastDelegate OnFontAtlasBuilt;

    void Tick(float DeltaSeconds);

    void RebuildFontAtlas();

private:
#if ENGINE_COMPATIBILITY_LEGACY_WORLD_ACTOR_TICK
    void OnWorldTickStart(ELevelTick TickType, float DeltaSeconds);
#endif
    void OnWorldTickStart(UWorld* World, ELevelTick TickType, float DeltaSeconds);

#if ENGINE_COMPATIBILITY_WITH_WORLD_POST_ACTOR_TICK
    void OnWorldPostActorTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);
#endif

#if WITH_EDITOR
    // Begin KLMod: Added world to constructor
    FContextData& GetEditorContextData(const UWorld& _World);
#endif

#if !WITH_EDITOR
    // Begin KLMod: Removed unused function
    // FContextData& GetStandaloneWorldContextData();
#endif

    FContextData& GetWorldContextData(const UWorld& World, int32* OutContextIndex = nullptr);

    void SetDPIScale(const FImGuiDPIScaleInfo& ScaleInfo);
    void BuildFontAtlas(const TMap<FName, TSharedPtr<ImFontConfig>>& CustomFontConfigs = {});

    TMap<int32, FContextData> Contexts;

    ImFontAtlas                     FontAtlas;
    TArray<TUniquePtr<ImFontAtlas>> FontResourcesToRelease;

    FImGuiModuleSettings& Settings;

    float DPIScale                      = -1.f;
    int32 FontResourcesReleaseCountdown = 0;

    //@Begin KLMod:
public:
    // Moved this from constructor so we can call it after we bound the delegate
    void Init();
    // Moved this from constructor so we can call it after we bound the delegate
    void RegisterDelegates();
    // Moved this from constructor so we can call it after we bound the delegate
    void UnregisterDelegates();

public:
    FOnBuildFontAtlas mOnBuildFontAtlasDelegate;
    //@End KLMod
};
