// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ImGuiContextManager.h"

#include "Fonts/Cousine_Regular.cpp"
#include "Fonts/Droid_Sans.cpp"
#include "Fonts/Karla_Regular.cpp"
#include "Fonts/Proggy_Tiny.cpp"
#include "Fonts/Roboto_Medium.cpp"
#include "ImGuiDelegatesContainer.h"
#include "ImGuiModuleSettings.h"
#include "ThirdPartyBuildImGui.h"
#include "ThirdPartyBuildNetImgui.h"
#include "UnrealImGuiModule.h"
#include "Utilities/WorldContext.h"
#include "Utilities/WorldContextIndex.h"
#include "HAL/Platform.h"

#include <imgui.h>
// TODO: Refactor ImGui Context Manager, to handle different types of worlds.

namespace
{
#if WITH_EDITOR

    // Name for editor ImGui context.
    FORCEINLINE FString GetEditorContextName()
    {
        return TEXT("Editor");
    }

    // Name for world ImGui context.
    FORCEINLINE FString GetWorldContextName(const UWorld& World)
    {
        using namespace Utilities;

        const FWorldContext* WorldContext = GetWorldContext(World);
        switch (WorldContext->WorldType)
        {
            case EWorldType::PIE:
                return FString::Printf(TEXT("PIEContext%d"), GetWorldContextIndex(*WorldContext));
            case EWorldType::Game:
                return TEXT("Game");
            case EWorldType::Editor:
                return TEXT("Editor");
            default:
                return TEXT("Other");
        }
    }

#else

    FORCEINLINE FString GetWorldContextName()
    {
        return TEXT("Game");
    }

    FORCEINLINE FString GetWorldContextName(const UWorld&)
    {
        return TEXT("Game");
    }

#endif  // WITH_EDITOR
}  // namespace

FImGuiContextManager::FImGuiContextManager(FImGuiModuleSettings& InSettings)
    : Settings(InSettings)
{
    Settings.OnDPIScaleChangedDelegate.AddRaw(this, &FImGuiContextManager::SetDPIScale);

    SetDPIScale(Settings.GetDPIScaleInfo());
}

FImGuiContextManager::~FImGuiContextManager()
{
    Settings.OnDPIScaleChangedDelegate.RemoveAll(this);

    //@Begin KLMod:
    UnregisterDelegates();
}

void FImGuiContextManager::Tick(float DeltaSeconds)
{
    NetImguiUpdate(Contexts);

    // In editor, worlds can get invalid. We could remove corresponding entries, but that would mean resetting ImGui
    // context every time when PIE session is restarted. Instead we freeze contexts until their worlds are re-created.

    for (auto& Pair : Contexts)
    {
        auto& ContextData = Pair.Value;
        //@Begin KLMod: tick the editor world only if is the only world. If we are plying do not tick it
        // Change code to allow to do so
        if (ContextData.PIEInstance < 0 && Contexts.Num() > 1)
        {
            continue;
        }

        if (ContextData.CanTick())
        {
            ContextData.ContextProxy->Tick(DeltaSeconds);
        }
        else
        {
            // Clear to make sure that we don't store objects registered for world that is no longer valid.
            FImGuiDelegatesContainer::Get().OnWorldDebug(Pair.Key).Clear();
        }
    }

    // Once all context tick they should use new fonts and we can release the old resources. Extra countdown is added
    // wait for contexts that ticked outside of this function, before rebuilding fonts.
    if (FontResourcesReleaseCountdown > 0 && !--FontResourcesReleaseCountdown)
    {
        FontResourcesToRelease.Empty();
    }
}

#if ENGINE_COMPATIBILITY_LEGACY_WORLD_ACTOR_TICK
void FImGuiContextManager::OnWorldTickStart(ELevelTick TickType, float DeltaSeconds)
{
    OnWorldTickStart(GWorld, TickType, DeltaSeconds);
}
#endif

void FImGuiContextManager::OnWorldTickStart(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
    if (World && (World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE || World->WorldType == EWorldType::Editor))
    {
        FImGuiContextProxy& ContextProxy = GetWorldContextProxy(*World);

        // Set as current, so we have right context ready when updating world objects.
        ContextProxy.SetAsCurrent();

        ContextProxy.DrawEarlyDebug();
#if !ENGINE_COMPATIBILITY_WITH_WORLD_POST_ACTOR_TICK
        ContextProxy.DrawDebug();
#endif
    }
}

#if ENGINE_COMPATIBILITY_WITH_WORLD_POST_ACTOR_TICK
void FImGuiContextManager::OnWorldPostActorTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
    if (World && (World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE || World->WorldType == EWorldType::Editor))
    {
        GetWorldContextProxy(*World).DrawDebug();
    }
}
#endif  // ENGINE_COMPATIBILITY_WITH_WORLD_POST_ACTOR_TICK

#if WITH_EDITOR
FContextData& FImGuiContextManager::GetEditorContextData(const UWorld& _World)
{
    FContextData* Data = Contexts.Find(Utilities::EDITOR_CONTEXT_INDEX);

    if (UNLIKELY(!Data))
    {
        Data = &Contexts.Emplace(Utilities::EDITOR_CONTEXT_INDEX, FContextData{_World, GetEditorContextName(), Utilities::EDITOR_CONTEXT_INDEX, FontAtlas, DPIScale, -1});
        OnContextProxyCreated.Broadcast(Utilities::EDITOR_CONTEXT_INDEX, *Data->ContextProxy);
    }

    return *Data;
}
#endif  // WITH_EDITOR

#if !WITH_EDITOR
// Begin KLMod: Removed unused function
// FContextData& FImGuiContextManager::GetStandaloneWorldContextData()
//{
//	FContextData* Data = Contexts.Find(Utilities::STANDALONE_GAME_CONTEXT_INDEX);
//
//	if (UNLIKELY(!Data))
//	{
//		Data = &Contexts.Emplace(Utilities::STANDALONE_GAME_CONTEXT_INDEX, FContextData{ GetWorldContextName(), Utilities::STANDALONE_GAME_CONTEXT_INDEX, FontAtlas, DPIScale });
//		OnContextProxyCreated.Broadcast(Utilities::STANDALONE_GAME_CONTEXT_INDEX, *Data->ContextProxy);
//	}
//
//	return *Data;
//}
#endif  // !WITH_EDITOR

FContextData& FImGuiContextManager::GetWorldContextData(const UWorld& World, int32* OutIndex)
{
    using namespace Utilities;

#if WITH_EDITOR
    // Default to editor context for anything other than a game world.
    if (World.WorldType != EWorldType::Game && World.WorldType != EWorldType::PIE)
    {
        if (OutIndex)
        {
            *OutIndex = Utilities::EDITOR_CONTEXT_INDEX;
        }

        //@Begin KLMod pass world
        return GetEditorContextData(World);
    }
#endif

    const FWorldContext* WorldContext = GetWorldContext(World);
    const int32          Index        = GetWorldContextIndex(*WorldContext);

    checkf(Index != Utilities::INVALID_CONTEXT_INDEX, TEXT("Couldn't find context index for world %s: WorldType = %d"),
           *World.GetName(), static_cast<int32>(World.WorldType));

#if WITH_EDITOR
    checkf(!GEngine->IsEditor() || Index != Utilities::EDITOR_CONTEXT_INDEX,
           TEXT("Context index %d is reserved for editor and cannot be used for world %s: WorldType = %d, NetMode = %d"),
           Index, *World.GetName(), static_cast<int32>(World.WorldType), static_cast<int32>(World.GetNetMode()));
#endif

    FContextData* Data = Contexts.Find(Index);

#if WITH_EDITOR
    if (UNLIKELY(!Data))
    {
        // Begin KLMod: Added world to constructor
        Data = &Contexts.Emplace(Index, FContextData{World, GetWorldContextName(World), Index, FontAtlas, DPIScale, WorldContext->PIEInstance});
        OnContextProxyCreated.Broadcast(Index, *Data->ContextProxy);
    }
    else
    {
        // Because we allow (for the sake of continuity) to map different PIE instances to the same index.
        Data->PIEInstance = WorldContext->PIEInstance;

        // Begin KLMod update also the world otherwise we are pointing to the one craeted in a previous pie session which is invalid
        Data->UpdateProxyWorld(World);
        // End KLMod
    }
#else
    if (UNLIKELY(!Data))
    {
        // Begin KLMod: Added world to constructor
        Data = &Contexts.Emplace(Index, FContextData{World, GetWorldContextName(World), Index, FontAtlas, DPIScale});
        OnContextProxyCreated.Broadcast(Index, *Data->ContextProxy);
    }
#endif

    // Begin KLMod: if we are in a network session, the world can be updated so check it here
    if (Data->ContextProxy.IsValid() && Data->ContextProxy->GetWorld() != &World)
    {
        Data->ContextProxy->UpdateWorld(World);
    }

    //End KLMod

    if (OutIndex)
    {
        *OutIndex = Index;
    }
    return *Data;
}

void FImGuiContextManager::SetDPIScale(const FImGuiDPIScaleInfo& ScaleInfo)
{
    const float Scale = ScaleInfo.GetImGuiScale();
    if (DPIScale != Scale)
    {
        DPIScale = Scale;

        // Only rebuild font atlas if it is already built. Otherwise allow the other logic to pick a moment.
        if (FontAtlas.IsBuilt())
        {
            RebuildFontAtlas();
        }

        for (auto& Pair : Contexts)
        {
            if (Pair.Value.ContextProxy)
            {
                Pair.Value.ContextProxy->SetDPIScale(DPIScale);
            }
        }
    }
}

void FImGuiContextManager::BuildFontAtlas(const TMap<FName, TSharedPtr<ImFontConfig>>& CustomFontConfigs)
{
    if (!FontAtlas.IsBuilt())
    {
        //@Begin KLMod:
        if (mOnBuildFontAtlasDelegate.IsBound())
        {
            mOnBuildFontAtlasDelegate.Execute(DPIScale, FontAtlas);
        }
        else
        {
            // revert to original way of build atlas
            ensureMsgf(false, TEXT("we should not enter here KLMod"));

            //---------------------------------------------------------------------------------------------
            // Load our Font (Must be loaded in same order as FImguiModule::eFont enum)
            ImFontConfig FontConfig = {};
            FontConfig.SizePixels   = FMath::RoundFromZero(13.f * DPIScale);
            FontAtlas.AddFontDefault(&FontConfig);
            FPlatformString::Strcpy(FontConfig.Name, sizeof(FontConfig.Name), "Roboto Medium, 16px");
            FontAtlas.AddFontFromMemoryCompressedTTF(Roboto_Medium_compressed_data, Roboto_Medium_compressed_size, 16.0f * DPIScale, &FontConfig);
            FPlatformString::Strcpy(FontConfig.Name, sizeof(FontConfig.Name), "Cousine Regular, 15px");
            FontAtlas.AddFontFromMemoryCompressedTTF(Cousine_Regular_compressed_data, Cousine_Regular_compressed_size, 15.0f * DPIScale, &FontConfig);
            FPlatformString::Strcpy(FontConfig.Name, sizeof(FontConfig.Name), "Karla Regular, 16px");
            FontAtlas.AddFontFromMemoryCompressedTTF(Karla_Regular_compressed_data, Karla_Regular_compressed_size, 16.0f * DPIScale, &FontConfig);
            FPlatformString::Strcpy(FontConfig.Name, sizeof(FontConfig.Name), "Droid Sans, 16px");
            FontAtlas.AddFontFromMemoryCompressedTTF(Droid_Sans_compressed_data, Droid_Sans_compressed_size, 16.0f * DPIScale, &FontConfig);
            FPlatformString::Strcpy(FontConfig.Name, sizeof(FontConfig.Name), "Proggy Tiny, 10px");
            FontAtlas.AddFontFromMemoryCompressedTTF(Proggy_Tiny_compressed_data, Proggy_Tiny_compressed_size, 10.0f * DPIScale, &FontConfig);

            // ... add extra fonts here (and add extra entry in 'FImguiModule::eFont' enum)

            // Build custom fonts
            for (const TPair<FName, TSharedPtr<ImFontConfig>>& CustomFontPair : CustomFontConfigs)
            {
                FName                    CustomFontName   = CustomFontPair.Key;
                TSharedPtr<ImFontConfig> CustomFontConfig = CustomFontPair.Value;

                // Set font name for debugging
                if (CustomFontConfig.IsValid())
                {
#if PLATFORM_WINDOWS
                    strcpy_s(CustomFontConfig->Name, 40, TCHAR_TO_ANSI(*CustomFontName.ToString()));
#endif
                }

                FontAtlas.AddFont(CustomFontConfig.Get());
            }
        }
        //@End KLMod

        unsigned char* Pixels;
        int            Width, Height, Bpp;
        FontAtlas.GetTexDataAsRGBA32(&Pixels, &Width, &Height, &Bpp);

        OnFontAtlasBuilt.Broadcast();
    }
}

void FImGuiContextManager::RebuildFontAtlas()
{
    if (FontAtlas.IsBuilt())
    {
        // Keep the old resources alive for a few frames to give all contexts a chance to bind to new ones.
        FontResourcesToRelease.Add(TUniquePtr<ImFontAtlas>(new ImFontAtlas()));
        Swap(*FontResourcesToRelease.Last(), FontAtlas);

        // Typically, one frame should be enough but since we allow for custom ticking, we need at least to frames to
        // wait for contexts that already ticked and will not do that before the end of the next tick of this manager.
        FontResourcesReleaseCountdown = 3;
    }

    BuildFontAtlas(FUnrealImGuiModule::Get().GetProperties().GetCustomFonts());
}

//@Begin KLMod:

void FImGuiContextManager::Init()
{
    BuildFontAtlas();
}

void FImGuiContextManager::RegisterDelegates()
{
    FWorldDelegates::OnWorldTickStart.AddRaw(this, &FImGuiContextManager::OnWorldTickStart);
#if ENGINE_COMPATIBILITY_WITH_WORLD_POST_ACTOR_TICK
    FWorldDelegates::OnWorldPostActorTick.AddRaw(this, &FImGuiContextManager::OnWorldPostActorTick);
#endif
}

void FImGuiContextManager::UnregisterDelegates()
{
    // Order matters because contexts can be created during World Tick Start events.
    FWorldDelegates::OnWorldTickStart.RemoveAll(this);
#if ENGINE_COMPATIBILITY_WITH_WORLD_POST_ACTOR_TICK
    FWorldDelegates::OnWorldPostActorTick.RemoveAll(this);
#endif
}

//@End KLMod
