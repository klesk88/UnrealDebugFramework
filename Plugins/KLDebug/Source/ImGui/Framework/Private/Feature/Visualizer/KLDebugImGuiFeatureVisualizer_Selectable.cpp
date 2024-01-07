// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Selectable.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"
#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

// ImGuiThirdParty module
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

    const FKLDebugImGuiFeatureContextInput ContextInput{ _Context.GetCurrentNetMode(), *mObject.Get() };
    mTreeVisualizer.DrawImGuiTree(EImGuiInterfaceType::SELECTABLE, ContextInput, _Context, mObject.Get(), mSelectedFeaturesIndexes);

    ImGui::TreePop();
}

void FKLDebugImGuiFeatureVisualizer_Selectable::DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, bool& _RequireCanvasDrawing)
{
#if DO_ENSURE
    if (!IsValid())
    {
        ensureMsgf(false, TEXT("should be valid when reached here"));
        return;
    }
#endif

    UObject& Object = *mObject.Get();

    auto Callback = [&_Context, &Object, &_RequireCanvasDrawing](FKLDebugImGuiFeatureVisualizerIterator& Iterator, FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> bool {
        IKLDebugImGuiFeatureInterface_Selectable& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Selectable>();
        _RequireCanvasDrawing |= Interface.RequireCanvasUpdate();
        const FKLDebugImGuiFeatureImGuiInput_Selectable FeatureContext{ _Context.GetWorld(), _Entry.GetIsEnableRef(), _Entry.TryGetFeatureContextMutable(), Object };
        Interface.DrawImGui(FeatureContext);
        return _Entry.IsEnable();
    };

    DrawImguiFeaturesEnabledCommon(_Context, Callback, &Object);
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