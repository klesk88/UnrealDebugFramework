// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

#include "Config/KLDebugImGuiConfig.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "Feature/Helpers/KLDebugFrameworkFeatureHelpers.h"
#include "Feature/Input/KLDebugImGuiGatherFeatureInput.h"

#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"
#include "KLDebugImGuiFrameworkModule.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "Window/KLDebugImGuiWindow.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"
#include "ImGui/User/Public/Feature/Interface/Canvas/KLDebugImGuiFeatureCanvasInput.h"
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

bool UKLDebugImGuiWorldSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
    return UKLDebugImGuiEngineSubsystem::Get() != nullptr;
}

void UKLDebugImGuiWorldSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    const UKLDebugImGuiConfig& ImGuiConfig = UKLDebugImGuiConfig::Get();
    mImGuiWindow = ImGuiConfig.GetImGuiWindow();

    if (mImGuiWindow.IsValid())
    {
        FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
        ImGuiWindow.Init();
    }
}

void UKLDebugImGuiWorldSubsystem::Deinitialize()
{
    Super::Deinitialize();

    FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
    ImGuiWindow.Shutdown();

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

void UKLDebugImGuiWorldSubsystem::DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_DrawImGui);

    ensureMsgf(&_CurrentWorldUpdated == GetWorld(), TEXT("we are updating the wrong world"));

    FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
    ImGuiWindow.Update(_CurrentWorldUpdated);

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

        bool RequireCanvasUpdate = false;
        DrawImGuiVisualizers(_CurrentWorldUpdated, _ContainerManager, RequireCanvasUpdate);
        DrawImguiSelectedObjects(_CurrentWorldUpdated, _ContainerManager, RequireCanvasUpdate);

        if (RequireCanvasUpdate)
        {
            RegisterCanvasCallback(_CurrentWorldUpdated);
        }
        else
        {
            UnregisterCanvasCallback(_CurrentWorldUpdated);
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

void UKLDebugImGuiWorldSubsystem::DrawImGuiVisualizers(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, bool& _RequireCanvasUpdate)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_DrawImGuiVisualizers);

    if (!mUniqueFeaturesVisualizer.IsValid())
    {
        return;
    }

    const FKLDebugImGuiFeatureVisualizerImGuiContext Context{ _World, true, mOnFeaturesUpdatedDelegate, _ContainerManager };
    mUniqueFeaturesVisualizer.DrawImGui(Context, _RequireCanvasUpdate);
}

void UKLDebugImGuiWorldSubsystem::DrawImguiSelectedObjects(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, bool& _RequireCanvasUpdate)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_DrawImguiSelectedObjects);

    if (mSelectedObjectsVisualizers.IsEmpty())
    {
        return;
    }

    if (ImGui::TreeNode("Selected_Objects"))
    {
        DrawImGuiObjects(_World, true, _ContainerManager, _RequireCanvasUpdate);
        ImGui::TreePop();
    }
    else
    {
        DrawImGuiObjects(_World, false, _ContainerManager, _RequireCanvasUpdate);
    }
}

void UKLDebugImGuiWorldSubsystem::DrawImGuiObjects(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, bool& _RequireCanvasUpdate)
{
    ImGui::Indent();

    const FKLDebugImGuiFeatureVisualizerImGuiContext Context{ _World, _DrawTree, mOnFeaturesUpdatedDelegate, _ContainerManager };
    for (FKLDebugImGuiFeatureVisualizer_Selectable& ObjVisualizer : mSelectedObjectsVisualizers)
    {
        if (!ObjVisualizer.IsValid())
        {
            continue;
        }

        ObjVisualizer.DrawImGui(Context, _RequireCanvasUpdate);
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
    if (!_Canvas || !GEngine->GetSmallFont())
    {
        return;
    }

    const FKLDebugImGuiFeaturesTypesContainerManager& FeaturesContainerManager = UKLDebugImGuiEngineSubsystem::Get()->GetFeatureContainerManager();

    UWorld& _World = *GetWorld();

    FKLDebugImGuiFeatureCanvasInput CanvasInput{ *_Canvas, *GEngine->GetSmallFont() };
    CanvasInput.CursorX = CanvasInput.DefaultX = 10.f;
    CanvasInput.CursorY = CanvasInput.DefaultY = 30.f;
    CanvasInput.World = &_World;

    mUniqueFeaturesVisualizer.DrawCanvas(FeaturesContainerManager, _World, CanvasInput);
    for (const FKLDebugImGuiFeatureVisualizer_Selectable& SelectableFeature : mSelectedObjectsVisualizers)
    {
        if (const UObject* OwnedObject = SelectableFeature.GetObjectKey().ResolveObjectPtr())
        {
            SelectableFeature.DrawCanvas(FeaturesContainerManager, *OwnedObject, CanvasInput);
        }
    }
}
