// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

#include "BottomBar/Manager/KLDebugFrameworkBottomBarManager.h"
#include "Config/KLDebugImGuiConfig.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "Feature/Delegates/KLDebugImGuiFeaturesTickInput.h"
#include "Feature/Helpers/KLDebugFrameworkFeatureHelpers.h"
#include "Feature/Input/KLDebugImGuiGatherFeatureInput.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"
#include "KLDebugImGuiFrameworkModule.h"
#include "Mode/Manager/KLDebugFrameworkModeManager.h"
#include "Rendering/KLDebugFrameworkRenderingComponent.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"
#include "Debug/DebugDrawService.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Stats/Stats2.h"

#if DO_ENSURE
// engine
#include "Misc/ScopeTryLock.h"
#endif

#define KLDEBUG_RENDER_LOCK_CHECK                                      \
    FScopeTryLock EnsureRenderLock{ &mEnsureCriticalSectionRenderer }; \
    ensureMsgf(EnsureRenderLock.IsLocked(), TEXT("MT issue with render cmp"));

bool UKLDebugImGuiWorldSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
    return UKLDebugImGuiEngineSubsystem::Get() != nullptr;
}

void UKLDebugImGuiWorldSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    const UKLDebugImGuiConfig& ImGuiConfig = UKLDebugImGuiConfig::Get();
    mImGuiWindow.Init();
}

void UKLDebugImGuiWorldSubsystem::Deinitialize()
{
    Super::Deinitialize();

    mImGuiWindow.Shutdown();

    if (KL::Debug::ImGui::Framework::OnImGuiWorldSusbsytemStateChange.IsBound())
    {
        KL::Debug::ImGui::Framework::OnImGuiWorldSusbsytemStateChange.Broadcast(false, *this);
    }

    const UWorld& World = *GetWorld();

    FKLDebugImGuiFeaturesTypesContainerManager& FeaturesContainerManager = UKLDebugImGuiEngineSubsystem::GetMutable()->GetContainerManagerMutable();
    mUniqueFeaturesVisualizer.Shutdown(World, FeaturesContainerManager, nullptr);

    for (FKLDebugImGuiFeatureVisualizer_Selectable& SelectableFeature : mSelectedObjectsVisualizers)
    {
        UObject* OwnedObject = SelectableFeature.GetObjectKey().ResolveObjectPtr();
        SelectableFeature.Shutdown(World, FeaturesContainerManager, OwnedObject);
    }

    if (mOnFeatureUpdateDelegateHandle.IsValid())
    {
        mOnFeaturesUpdatedDelegate.Remove(mOnFeatureUpdateDelegateHandle);
        mOnFeatureUpdateDelegateHandle.Reset();
    }

    if (RenderingCmp && RenderingCmp->IsRegistered())
    {
        RenderingCmp->UnregisterComponent();
    }
}

void UKLDebugImGuiWorldSubsystem::OnWorldBeginPlay(UWorld& _World)
{
    if (!UKismetSystemLibrary::IsDedicatedServer(&_World))
    {
        const UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
        const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = EngineSusbsytem->GetFeatureContainerManager();

        mImGuiTreeName = FString::Format(TEXT("{0} {1}"), { *GetDebugStringForWorld(&_World), *_World.GetName() });

        const FKLDebugImGuiFeatureContainerBase& FeaturesContainer = FeatureContainerManager.GetContainerMutable(EImGuiInterfaceType::UNIQUE);
        TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> Features;
        FeaturesContainer.GatherFeatures(_World, Features);

        if (!Features.IsEmpty())
        {
            mUniqueFeaturesVisualizer.Init(FeaturesContainer, TEXT("Systems"), MoveTemp(Features));
        }

        RenderingCmp = NewObject<UKLDebugFrameworkRenderingComponent>(this, TEXT("KLDebugRenderingCmp"));
        mOnFeatureUpdateDelegateHandle = mOnFeaturesUpdatedDelegate.AddUObject(this, &UKLDebugImGuiWorldSubsystem::OnFeatureStatusUpdate);
        mUpdateSystems.Init(0, static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Count));

        mShouldTick = true;
    }

    if (KL::Debug::ImGui::Framework::OnImGuiWorldSusbsytemStateChange.IsBound())
    {
        KL::Debug::ImGui::Framework::OnImGuiWorldSusbsytemStateChange.Broadcast(true, *this);
    }
}

UKLDebugImGuiWorldSubsystem* UKLDebugImGuiWorldSubsystem::TryGetMutable(const UObject& _Object)
{
    const UWorld* World = GEngine->GetWorldFromContextObject(&_Object, EGetWorldErrorMode::ReturnNull);
    if (World)
    {
        return World->GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    }

    return nullptr;
}

void UKLDebugImGuiWorldSubsystem::OnObjectSelected(UObject& _Object)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_OnObjectSelected);

    const UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    if (!EngineSusbsytem->IsImGuiSystemEnabled())
    {
        return;
    }

    const int32 Index = mSelectedObjectsVisualizers.IndexOfByKey(_Object);
    if (Index != INDEX_NONE)
    {
        return;
    }

    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> Features;
    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = EngineSusbsytem->GetFeatureContainerManager();
    const FKLDebugImGuiFeatureContainerBase& SelectedObjectFeatures = FeatureContainerManager.GetContainer(EImGuiInterfaceType::SELECTABLE);

    SelectedObjectFeatures.GatherFeatures(_Object, Features);
    mSelectedObjectsVisualizers.Emplace(SelectedObjectFeatures, EngineSusbsytem->GetOverlayMaterial(), _Object, MoveTemp(Features));
}

void UKLDebugImGuiWorldSubsystem::TryGatherFeatureAndContext(FKLDebugImGuiGatherFeatureInput& _Input) const
{
    FKLDebugImGuiFeatureContext_Base* FeatureContext = nullptr;
    IKLDebugImGuiFeatureInterfaceBase* Interface = nullptr;
    bool IsFeatureStillValid = false;

    switch (_Input.GetContainerType())
    {
    case EImGuiInterfaceType::SELECTABLE:
        if (const UObject* Object = _Input.TryGetObject())
        {
            const FKLDebugImGuiFeatureVisualizer_Selectable* ObjectFeatures = mSelectedObjectsVisualizers.FindByKey(*Object);
            const FKLDebugImGuiFeatureVisualizerEntry* VisualizerEntry = ObjectFeatures ? ObjectFeatures->TryGetSelectedFeature(_Input.GetFeatureIndex()) : nullptr;
            if (VisualizerEntry)
            {
                FeatureContext = VisualizerEntry->TryGetFeatureContextMutable();
                IsFeatureStillValid = true;
            }
        }
        break;
    case EImGuiInterfaceType::UNIQUE:
    {
        const FKLDebugImGuiFeatureVisualizerEntry* VisualizerEntry = mUniqueFeaturesVisualizer.TryGetSelectedFeature(_Input.GetFeatureIndex());
        if (VisualizerEntry)
        {
            IsFeatureStillValid = true;
            FeatureContext = VisualizerEntry->TryGetFeatureContextMutable();
        }
    }
    break;
    case EImGuiInterfaceType::COUNT:
        ensureMsgf(false, TEXT("should never enter here"));
        break;
    }

    if (IsFeatureStillValid)
    {
        const FKLDebugImGuiFeaturesTypesContainerManager& ContainerManager = _Input.GetContainerManager();
        FKLDebugImGuiFeatureContainerBase& Container = ContainerManager.GetContainerMutable(_Input.GetContainerType());
        Interface = &Container.GetFeatureMutable(_Input.GetFeatureIndex());
    }

    _Input.SetFeatureData(Interface, FeatureContext);
}

void UKLDebugImGuiWorldSubsystem::TryGatherSceneProxies(const UPrimitiveComponent& _RenderingComponent, const KL::Debug::Framework::Rendering::GatherSceneProxyCallback& _Callback)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiWorldSubsystem_TryGatherSceneProxies);

#if DO_ENSURE
    KLDEBUG_RENDER_LOCK_CHECK
#endif

    FKLDebugImGuiFeaturesTypesContainerManager& FeaturesContainerManager = UKLDebugImGuiEngineSubsystem::GetMutable()->GetContainerManagerMutable();
    mUniqueFeaturesVisualizer.GatherSceneProxies(_RenderingComponent, _Callback, FeaturesContainerManager);

    for (FKLDebugImGuiFeatureVisualizer_Selectable& SelectableFeature : mSelectedObjectsVisualizers)
    {
        SelectableFeature.GatherSceneProxies(_RenderingComponent, _Callback, FeaturesContainerManager);
    }
}

IKLDebugContextInterface* UKLDebugImGuiWorldSubsystem::GetCurrentWindowData(const EKLDebugWindowTypes _WindowType, int32& _Index) const
{
    _Index = -1;

    switch (_WindowType)
    {
    case EKLDebugWindowTypes::BottomBar:
        _Index = mImGuiWindow.GetCurrentBottomBarIndex();
        return mImGuiWindow.GetCurrentBottomBarContext();
        break;
    case EKLDebugWindowTypes::Mode:
        _Index = mImGuiWindow.GetCurrentModeIndex();
        return mImGuiWindow.GetCurrentModeContext();
        break;
    default:
        ensureMsgf(false, TEXT("not supported"));
        break;
    }

    return nullptr;
}

void UKLDebugImGuiWorldSubsystem::Tick(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, FKLDebugFrameworkBottomBarManager& _BottomBarManager, FKLDebugFrameworkModeManager& _ModeManager)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_Tick);

#if DO_ENSURE
    KLDEBUG_RENDER_LOCK_CHECK
#endif

    UpdateCanvasAndRenderStatus();
    bool ShouldTickDelegates = false;
    if (mOnShouldTickDelegate.IsBound())
    {
        mOnShouldTickDelegate.Broadcast(ShouldTickDelegates);
    }

    if (mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Tick)] == 0 && !ShouldTickDelegates)
    {
        mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy)] = 0;
        return;
    }

    KL::Debug::ImGui::Features::Types::FeatureEnableSet SystemEnable;

    if (mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Tick)] != 0)
    {
        if (mUniqueFeaturesVisualizer.IsValid())
        {
            mUniqueFeaturesVisualizer.TickFeatures(_CurrentWorldUpdated, _ContainerManager, SystemEnable);
        }

        for (int32 i = mSelectedObjectsVisualizers.Num() - 1; i >= 0; --i)
        {
            FKLDebugImGuiFeatureVisualizer_Selectable& SelectableFeature = mSelectedObjectsVisualizers[i];
            if (!SelectableFeature.IsValid())
            {
                mSelectedObjectsVisualizers.RemoveAtSwap(i, 1, false);
                continue;
            }

            SelectableFeature.TickFeatures(_CurrentWorldUpdated, _ContainerManager, SystemEnable);
        }
    }

    if (ShouldTickDelegates)
    {
        IKLDebugBottomBarInterface* BottomBarInterface = _BottomBarManager.TryGetCurrentInterfaceMutable(mImGuiWindow.GetCurrentBottomBarIndex());
        IKLDebugModeInterface* ModeInterface = _ModeManager.TryGetCurrentInterfaceMutable(mImGuiWindow.GetCurrentModeIndex());

        const FKLDebugImGuiFeaturesTickInput Input{ _CurrentWorldUpdated,
            mImGuiWindow.GetCurrentModeIndex(),
            mImGuiWindow.GetCurrentBottomBarIndex(),
            mUniqueFeaturesVisualizer,
            mSelectedObjectsVisualizers,
            _ContainerManager,
            SystemEnable,
            ModeInterface,
            mImGuiWindow.GetCurrentModeContext(),
            BottomBarInterface,
            mImGuiWindow.GetCurrentBottomBarContext() };

        mOnFeaturesTick.Broadcast(Input);
    }

    const bool UpdateSceneProxy = SystemEnable[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy)] || mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy)] > 0;
    if (RenderingCmp && RenderingCmp->IsRegistered() && UpdateSceneProxy)
    {
        RenderingCmp->MarkRenderStateDirty();
    }

    mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy)] = 0;
}

void UKLDebugImGuiWorldSubsystem::DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, FKLDebugFrameworkBottomBarManager& _BottomBarManager, FKLDebugFrameworkModeManager& _ModeManager)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_DrawImGui);

#if DO_ENSURE
    KLDEBUG_RENDER_LOCK_CHECK
#endif

    ensureMsgf(&_CurrentWorldUpdated == GetWorld(), TEXT("we are updating the wrong world"));

    const int32 CurrentBottomBar = mImGuiWindow.GetCurrentBottomBarIndex();
    const int32 CurrentMode = mImGuiWindow.GetCurrentModeIndex();
    const IKLDebugBottomBarInterface* BottomBarInterface = _BottomBarManager.TryGetCurrentInterface(CurrentBottomBar);
    const IKLDebugModeInterface* CurrentModeInterface = _ModeManager.TryGetCurrentInterface(CurrentMode);
    const bool CurentModeRequireCanvasDraw = _ModeManager.RequireCanvasDraw(mImGuiWindow.GetCurrentModeIndex());
    mImGuiWindow.Update(_CurrentWorldUpdated, _BottomBarManager, _ModeManager);

    if (mOnWindowStatusChange.IsBound())
    {
        if (CurrentBottomBar != mImGuiWindow.GetCurrentBottomBarIndex())
        {
            const IKLDebugBottomBarInterface* NewBottomBarInterface = _BottomBarManager.TryGetCurrentInterface(mImGuiWindow.GetCurrentBottomBarIndex());
            const FName ID = NewBottomBarInterface ? NewBottomBarInterface->GetID() : NAME_None;
            mOnWindowStatusChange.Broadcast(mImGuiWindow.GetCurrentBottomBarIndex(), EKLDebugWindowTypes::BottomBar, ID, BottomBarInterface, NewBottomBarInterface);
        }

        if (CurrentMode != mImGuiWindow.GetCurrentModeIndex())
        {
            const IKLDebugModeInterface* NewModeInterface = _ModeManager.TryGetCurrentInterface(mImGuiWindow.GetCurrentModeIndex());
            const FName ID = NewModeInterface ? NewModeInterface->GetID() : NAME_None;
            mOnWindowStatusChange.Broadcast(mImGuiWindow.GetCurrentModeIndex(), EKLDebugWindowTypes::Mode, ID, CurrentModeInterface, NewModeInterface);
        }
    }

    if (CurrentMode != mImGuiWindow.GetCurrentModeIndex())
    {
        if (CurentModeRequireCanvasDraw)
        {
            mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Canvas)]--;
        }

        if (_ModeManager.RequireCanvasDraw(mImGuiWindow.GetCurrentModeIndex()))
        {
            mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Canvas)]++;
        }
    }

    if (mImGuiWindow.GetCurrentModeIndex() != -1)
    {
        _ModeManager.DrawImGui(mImGuiWindow.GetCurrentModeIndex(), _CurrentWorldUpdated, mImGuiWindow.GetCurrentModeContext(), mRenderModeWindow);
    }

    const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings;
    if (!ImGui::Begin(TCHAR_TO_ANSI(*mImGuiTreeName), nullptr, WindowFlags))
    {
        ImGui::End();
        return;
    }

    ImGui::Separator();

    const ImGuiTabBarFlags TabFlags = ImGuiTabBarFlags_::ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyMask_;
    if (!ImGui::BeginTabBar("##tabs", TabFlags))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabItem("FeaturesTree"))
    {
        ImGui::PushID(this);

        KL::Debug::ImGui::Features::Types::FeatureEnableSet FeaturesData;
        DrawImGuiVisualizers(_CurrentWorldUpdated, _ContainerManager, FeaturesData);
        DrawImguiSelectedObjects(_CurrentWorldUpdated, _ContainerManager, FeaturesData);

        if (FeaturesData[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy)])
        {
            mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy)] = 1;
        }

        ImGui::PopID();

        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();
}

void UKLDebugImGuiWorldSubsystem::Render(const UWorld& _CurrentWorldUpdated, const FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_Render);

    if (mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Render)] == 0)
    {
        return;
    }

    ensureMsgf(&_CurrentWorldUpdated == GetWorld(), TEXT("we are updating the wrong world"));

    const FKLDebugImGuiFeatureVisualizerRenderContext Context{ _CurrentWorldUpdated, _ContainerManager };

    if (mUniqueFeaturesVisualizer.IsValid())
    {
        mUniqueFeaturesVisualizer.Render(Context);
    }

    for (const FKLDebugImGuiFeatureVisualizer_Selectable& ObjVisualizer : mSelectedObjectsVisualizers)
    {
        if (!ObjVisualizer.IsValid())
        {
            continue;
        }

        ObjVisualizer.Render(Context);
    }
}

void UKLDebugImGuiWorldSubsystem::DrawImGuiVisualizers(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_DrawImGuiVisualizers);

    if (!mUniqueFeaturesVisualizer.IsValid())
    {
        return;
    }

    const FKLDebugImGuiFeatureVisualizerImGuiContext Context{ _World, true, mOnFeaturesUpdatedDelegate, _ContainerManager };
    mUniqueFeaturesVisualizer.DrawImGui(Context, _RequiredExternalSystem);
}

void UKLDebugImGuiWorldSubsystem::DrawImguiSelectedObjects(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_DrawImguiSelectedObjects);

    if (mSelectedObjectsVisualizers.IsEmpty())
    {
        return;
    }

    if (ImGui::TreeNode("Selected_Objects"))
    {
        DrawImGuiObjects(_World, true, _ContainerManager, _RequiredExternalSystem);
        ImGui::TreePop();
    }
    else
    {
        DrawImGuiObjects(_World, false, _ContainerManager, _RequiredExternalSystem);
    }
}

void UKLDebugImGuiWorldSubsystem::DrawImGuiObjects(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem)
{
    ImGui::Indent();

    const FKLDebugImGuiFeatureVisualizerImGuiContext Context{ _World, _DrawTree, mOnFeaturesUpdatedDelegate, _ContainerManager };
    for (FKLDebugImGuiFeatureVisualizer_Selectable& ObjVisualizer : mSelectedObjectsVisualizers)
    {
        if (!ObjVisualizer.IsValid())
        {
            continue;
        }

        ObjVisualizer.DrawImGui(Context, _RequiredExternalSystem);
    }

    ImGui::Unindent();

    const EImGuiInterfaceType SelectableContainerType = EImGuiInterfaceType::SELECTABLE;
    FKLDebugImGuiFeatureContainerBase& SelectableObjectsContainer = _ContainerManager.GetContainerMutable(SelectableContainerType);

    for (int32 i = mSelectedObjectsVisualizers.Num() - 1; i >= 0; --i)
    {
        const FKLDebugImGuiFeatureVisualizer_Selectable& ObjVisualizer = mSelectedObjectsVisualizers[i];
        if (!ObjVisualizer.IsValid() || !ObjVisualizer.ShouldKeepAlive())
        {
            if (mOnFeaturesUpdatedDelegate.IsBound() && !ObjVisualizer.GetFeaturesIndexes().IsEmpty())
            {
                FKLDebugImGuiSubsetFeaturesIterator Iterator = SelectableObjectsContainer.GetFeaturesSubsetIterator(ObjVisualizer.GetFeaturesIndexes());
                IKLDebugImGuiFeatureInterfaceBase& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterfaceBase>();
                KL::Debug::Feature::Helpers::OnFeatureUnselected(_World, ObjVisualizer.GetObjectKey().ResolveObjectPtr(), Interface);

                FKLDebugImGuiSubsetFeaturesConstIterator ConstIter = SelectableObjectsContainer.GetFeaturesSubsetConstIterator(ObjVisualizer.GetFeaturesIndexes());
                FKLDebugImGuiFeatureStatusUpdateData DelegateData{ _World, false, SelectableContainerType, ObjVisualizer.GetObjectKey(), ConstIter };
                DelegateData.SetFullyRemoved();
                mOnFeaturesUpdatedDelegate.Broadcast(DelegateData);
            }

            mSelectedObjectsVisualizers.RemoveAt(i, 1, false);
        }
    }
}

void UKLDebugImGuiWorldSubsystem::RegisterCanvasCallback(const UWorld& _World)
{
    if (mImGuiCanvasDelegateHandle.IsValid())
    {
        return;
    }

    const FKLDebugImGuiFrameworkModule& Module = FKLDebugImGuiFrameworkModule::Get();
    const int32 ImGuiFlagIndex = FEngineShowFlags::FindIndexByName(*Module.GetKLDebugEngineFlag());
    if (ImGuiFlagIndex == INDEX_NONE)
    {
        ensureMsgf(false, TEXT("we cant find out custom engine flag"));
        return;
    }

    UGameViewportClient* GameViewportClient = _World.GetGameViewport();
    if (!GameViewportClient)
    {
        return;
    }

    GameViewportClient->EngineShowFlags.SetSingleFlag(ImGuiFlagIndex, true);
    mImGuiCanvasDelegateHandle = UDebugDrawService::Register(*Module.GetKLDebugEngineFlag(), FDebugDrawDelegate::CreateUObject(this, &UKLDebugImGuiWorldSubsystem::DrawOnCanvas));
}

void UKLDebugImGuiWorldSubsystem::UnregisterCanvasCallback(const UWorld& _World)
{
    if (!mImGuiCanvasDelegateHandle.IsValid())
    {
        return;
    }

    UGameViewportClient* GameViewportClient = _World.GetGameViewport();
    if (GameViewportClient)
    {
        const FKLDebugImGuiFrameworkModule& Module = FKLDebugImGuiFrameworkModule::Get();
        const int32 ImGuiFlagIndex = FEngineShowFlags::FindIndexByName(*Module.GetKLDebugEngineFlag());
        GameViewportClient->EngineShowFlags.SetSingleFlag(ImGuiFlagIndex, false);
    }

    UDebugDrawService::Unregister(mImGuiCanvasDelegateHandle);
    mImGuiCanvasDelegateHandle.Reset();
}

void UKLDebugImGuiWorldSubsystem::DrawOnCanvas(UCanvas* _Canvas, [[maybe_unused]] APlayerController* _PlayerController)
{
    UFont* SmallFont = GEngine->GetSmallFont();
    if (!_Canvas || !SmallFont)
    {
        return;
    }

    UWorld& _World = *GetWorld();
    const FKLDebugImGuiFeaturesTypesContainerManager& FeaturesContainerManager = UKLDebugImGuiEngineSubsystem::Get()->GetFeatureContainerManager();
    mUniqueFeaturesVisualizer.DrawOnCanvas(FeaturesContainerManager, *_Canvas, *SmallFont, _World);
    for (const FKLDebugImGuiFeatureVisualizer_Selectable& SelectableFeature : mSelectedObjectsVisualizers)
    {
        SelectableFeature.DrawOnCanvas(FeaturesContainerManager, *_Canvas, *SmallFont, _World);
    }

    const FKLDebugFrameworkModeManager& ModeManager = UKLDebugImGuiEngineSubsystem::Get()->GetModeManager();
    ModeManager.DrawCanvas(mImGuiWindow.GetCurrentModeIndex(), _World, *_Canvas, *SmallFont, mImGuiWindow.GetCurrentModeContext());
}

void UKLDebugImGuiWorldSubsystem::RegisterRenderComponent() const
{
    if (!RenderingCmp->IsRegistered())
    {
        RenderingCmp->RegisterComponentWithWorld(GetWorld());
    }
}

void UKLDebugImGuiWorldSubsystem::UnregisterRenderComponent() const
{
    RenderingCmp->UnregisterComponent();
}

void UKLDebugImGuiWorldSubsystem::UpdateCanvasAndRenderStatus()
{
    UWorld& World = *GetWorld();
    if (mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Canvas)] > 0)
    {
        RegisterCanvasCallback(World);
    }
    else
    {
        UnregisterCanvasCallback(World);
    }

    if (mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::SceneProxy)] > 0)
    {
        RegisterRenderComponent();
    }
    else if (RenderingCmp->IsRegistered())
    {
        UnregisterRenderComponent();
    }
}

void UKLDebugImGuiWorldSubsystem::OnFeatureStatusUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData)
{
    FKLDebugImGuiSubsetFeaturesConstIterator& FeaturesIterator = _FeatureUpdateData.GetFeatureIterator();
    const int32 Value = _FeatureUpdateData.IsFeatureAdded() ? 1 : -1;

    for (; FeaturesIterator; ++FeaturesIterator)
    {
        const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = FeaturesIterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
        if (FeatureInterface.RequireCanvasUpdate())
        {
            mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Canvas)] += Value;
        }

        if (FeatureInterface.RequireSceneProxy())
        {
            mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::SceneProxy)] += Value;
            mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy)] = 1;
        }

        if (FeatureInterface.RequireTick())
        {
            mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Tick)] += Value;
        }

        if (FeatureInterface.RequireRender())
        {
            mUpdateSystems[static_cast<int32>(KL::Debug::ImGui::Features::Types::EFeatureEnableType::Render)] += Value;
        }
    }

#if DO_ENSURE
    for (const int32 SystemValue : mUpdateSystems)
    {
        ensureMsgf(SystemValue >= 0, TEXT("value must be positive"));
    }
#endif
}
