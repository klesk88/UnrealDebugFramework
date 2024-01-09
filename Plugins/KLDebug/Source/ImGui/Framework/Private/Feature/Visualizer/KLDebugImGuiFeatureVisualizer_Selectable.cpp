// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Selectable.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

// modules
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureSelectableAllInputs.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Components/MeshComponent.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInterface.h"

FKLDebugImGuiFeatureVisualizer_Selectable::FKLDebugImGuiFeatureVisualizer_Selectable(const FKLDebugImGuiFeatureContainerBase& _Container, UMaterialInterface* _MaterialInterface, UObject& _Object, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : FKLDebugImGuiFeatureVisualizerBase(_Container, MoveTemp(_FeaturesIndexes))
    , mObject(&_Object)
    , mObjectKey(&_Object)
{
    if (UMeshComponent* MeshCmp = TryGetMeshComponent())
    {
        mOriginalMaterialOverlay = MeshCmp->GetOverlayMaterial();
        if (mOriginalMaterialOverlay.IsValid())
        {
            mOriginalMaterialOverlay->SetInternalFlags(EInternalObjectFlags::Async);
        }
    }

    SetMaterialOverlay(_MaterialInterface);
}

FKLDebugImGuiFeatureVisualizer_Selectable::~FKLDebugImGuiFeatureVisualizer_Selectable()
{
    if (mOriginalMaterialOverlay.IsValid())
    {
        mOriginalMaterialOverlay->ClearInternalFlags(EInternalObjectFlags::Async);
    }

    SetMaterialOverlay(mOriginalMaterialOverlay.Get());
}

void FKLDebugImGuiFeatureVisualizer_Selectable::DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context)
{
#if DO_ENSURE
    if (!IsValid())
    {
        ensureMsgf(false, TEXT("should be valid when reached here"));
        return;
    }
#endif

    const FString ObjectName = mObject->GetFName().ToString();
    if (!ImGui::TreeNode(TCHAR_TO_ANSI(*ObjectName)))
    {
        ImGui::SameLine();
        if (ImGui::Button("Remove From List"))
        {
            mKeepAlive = false;
        }

        return;
    }

    if (ImGui::Button("Remove From List"))
    {
        mKeepAlive = false;
        return;
    }

    mTreeVisualizer.DrawImGuiTree(EImGuiInterfaceType::SELECTABLE, _Context, mObject.Get(), mSelectedFeaturesIndexes);

    ImGui::TreePop();
}

void FKLDebugImGuiFeatureVisualizer_Selectable::TickFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem)
{
    UObject& Object = *mObject.Get();

    auto Callback = [&_World, &Object, &_RequiredExternalSystem](FKLDebugImGuiFeatureVisualizerIterator& Iterator, const FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> void {
        IKLDebugImGuiFeatureInterface_Selectable& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Selectable>();
        FKLDebugFeatureTickInput_Selectable Input{ _World, Object, _Entry.TryGetFeatureContextMutable() };
        Interface.Tick(Input);
        KL::Debug::ImGui::Features::Types::UpdateFeatureEnableSet(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy, Input.ShouldUpdateSceneProxy(), _RequiredExternalSystem);
    };

    IterateOnSelectedFeaturesMutable(Callback, _FeatureContainerManager);
}

void FKLDebugImGuiFeatureVisualizer_Selectable::DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context)
{
#if DO_ENSURE
    if (!IsValid())
    {
        ensureMsgf(false, TEXT("should be valid when reached here"));
        return;
    }
#endif

    UObject& Object = *mObject.Get();

    auto Callback = [&_Context, &Object](FKLDebugImGuiFeatureVisualizerIterator& Iterator, FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> bool {
        IKLDebugImGuiFeatureInterface_Selectable& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Selectable>();
        const FKLDebugImGuiFeatureImGuiInput_Selectable FeatureContext{ _Context.GetWorld(), _Entry.GetIsEnableRef(), _Entry.TryGetFeatureContextMutable(), Object };
        Interface.DrawImGui(FeatureContext);
        return _Entry.IsEnable();
    };

    DrawImguiFeaturesEnabledCommon(_Context, Callback, &Object);
}

void FKLDebugImGuiFeatureVisualizer_Selectable::DrawOnCanvas(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, UCanvas& _Canvas, UFont& _Font, UWorld& _World) const
{
    if (!IsValid())
    {
        return;
    }

    const UObject& Object = *mObject.Get();
    auto Callback = [&Object, &_Canvas, &_Font, &_World](FKLDebugImGuiFeatureVisualizerConstIterator& Iterator, const FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> void {
        const IKLDebugImGuiFeatureInterface_Selectable& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Selectable>();
        FKLDebugFeatureDrawCanvasInput_Selectable Input{ Object, _Canvas, _Font, _Entry.TryGetFeatureContextMutable() };
        InitCommonCanvasInput(_World, Input);
        Interface.DrawOnCanvas(Input);
    };

    IterateOnSelectedFeatures(_FeatureContainerManager, Callback);
}

void FKLDebugImGuiFeatureVisualizer_Selectable::GatherSceneProxies(const UPrimitiveComponent& _RenderingComponent, const KL::Debug::Framework::Rendering::GatherSceneProxyCallback& _Callback, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
{
    if (!IsValid())
    {
        return;
    }

    auto Callback = [&_Callback, &_RenderingComponent](FKLDebugImGuiFeatureVisualizerConstIterator& Iterator, const FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> void {
        const IKLDebugImGuiFeatureInterface_Selectable& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Selectable>();
        FKLDebugFeatureSceneProxyInput_Selectable Input{ _RenderingComponent, _Entry.TryGetFeatureContextMutable() };
        TUniquePtr<FDebugRenderSceneProxy> SceneProxy = Interface.CreateDebugSceneProxy(Input);
        _Callback(MoveTemp(SceneProxy), Input.GetDrawDelegateHelper());
    };

    IterateOnSelectedFeatures(_FeatureContainerManager, Callback);
}

UMeshComponent* FKLDebugImGuiFeatureVisualizer_Selectable::TryGetMeshComponent() const
{
    if (!mObject.IsValid())
    {
        return nullptr;
    }

    UMeshComponent* MeshCmp = nullptr;
    if (const ACharacter* Character = Cast<const ACharacter>(mObject.Get()))
    {
        MeshCmp = Character->GetMesh();
    }
    else if (const AActor* Actor = Cast<const AActor>(mObject.Get()))
    {
        MeshCmp = Actor->FindComponentByClass<UMeshComponent>();
    }

    return MeshCmp;
}

void FKLDebugImGuiFeatureVisualizer_Selectable::SetMaterialOverlay(UMaterialInterface* _MaterialInterface)
{
    UMeshComponent* MeshCmp = TryGetMeshComponent();
    if (MeshCmp)
    {
        MeshCmp->SetOverlayMaterial(_MaterialInterface);
    }
}

void FKLDebugImGuiFeatureVisualizer_Selectable::Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const
{
    // NOTE: we allow the user to modify the context (if present)

#if DO_ENSURE
    if (!IsValid())
    {
        ensureMsgf(false, TEXT("should be valid when reached here"));
        return;
    }
#endif

    const UObject& Object = *mObject.Get();
    const FKLDebugImGuiFeatureContainerBase& FeatureContainer = _Context.GetFeaturesContainerManager().GetContainer(GetInterfaceType());
    FKLDebugImGuiFeatureVisualizerConstIterator Iterator = FeatureContainer.GetFeatureVisualizerConstIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        const FKLDebugImGuiFeatureVisualizerEntry& VisualizerData = Iterator.GetEntryData();
        const FKLDebugImGuiFeatureRenderInput_Selectable FeatureContext{ _Context.GetWorld(), Object, VisualizerData.TryGetFeatureContextMutable() };
        const IKLDebugImGuiFeatureInterface_Selectable& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Selectable>();
        Interface.Render(FeatureContext);
    }
}