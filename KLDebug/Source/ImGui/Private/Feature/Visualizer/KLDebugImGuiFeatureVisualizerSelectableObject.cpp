#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerSelectableObject.h"

#include "Feature/Container/Iterators/KLDebugImGuiSubsetFeaturesIterator.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Components/MeshComponent.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInterface.h"

FKLDebugImGuiFeatureVisualizerSelectableObject::FKLDebugImGuiFeatureVisualizerSelectableObject(const FKLDebugImGuiFeatureContainerBase& _Container, UMaterialInterface* _MaterialInterface, UObject& _Object, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : FKLDebugImGuiFeatureVisualizerBase(_Container, MoveTemp(_FeaturesIndexes))
    , mObject(&_Object)
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

FKLDebugImGuiFeatureVisualizerSelectableObject::~FKLDebugImGuiFeatureVisualizerSelectableObject()
{
    if (mOriginalMaterialOverlay.IsValid())
    {
        mOriginalMaterialOverlay->ClearInternalFlags(EInternalObjectFlags::Async);
    }

    SetMaterialOverlay(mOriginalMaterialOverlay.Get());
}

void FKLDebugImGuiFeatureVisualizerSelectableObject::DrawImGuiTree(const UWorld& _World)
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

    mTreeVisualizer.DrawImGuiTree(mSelectedFeaturesIndexes);

    ImGui::TreePop();
}

void FKLDebugImGuiFeatureVisualizerSelectableObject::DrawImGuiFeaturesEnabled(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
{
#if DO_ENSURE
    if (!IsValid())
    {
        ensureMsgf(false, TEXT("should be valid when reached here"));
        return;
    }
#endif

    UObject& Object = *mObject.Get();

    auto Callback = [&_World, &Object](FKLDebugImGuiFeatureVisualizerIterator& Iterator, FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> bool{
        IKLDebugImGuiFeatureInterface_SelectableObject& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_SelectableObject>();
        Interface.DrawImGui(_World, _Entry.GetIsEnableRef(), Object);
        return _Entry.IsEnable();
    };

    FKLDebugImGuiFeatureContainerBase& FeatureContainer = _FeatureContainerManager.GetContainerMutable(EContainerType::SELECTABLE_OBJECTS);
    DrawImguiFeaturesEnabledCommon(FeatureContainer, Callback);
}

UMeshComponent* FKLDebugImGuiFeatureVisualizerSelectableObject::TryGetMeshComponent() const
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
    else if(const AActor* Actor = Cast<const AActor>(mObject.Get()))
    {
        MeshCmp = Actor->FindComponentByClass<UMeshComponent>();
    }

    return MeshCmp;
}

void FKLDebugImGuiFeatureVisualizerSelectableObject::SetMaterialOverlay(UMaterialInterface* _MaterialInterface)
{
    UMeshComponent* MeshCmp = TryGetMeshComponent();
    if (MeshCmp)
    {
        MeshCmp->SetOverlayMaterial(_MaterialInterface);
    }
}

void FKLDebugImGuiFeatureVisualizerSelectableObject::Render(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) const
{
#if DO_ENSURE
    if (!IsValid())
    {
        ensureMsgf(false, TEXT("should be valid when reached here"));
        return;
    }
#endif

    const UObject&                           Object   = *mObject.Get();
    const FKLDebugImGuiFeatureContainerBase& FeatureContainer = _FeatureContainerManager.GetContainer(EContainerType::SELECTABLE_OBJECTS);
    FKLDebugImGuiFeatureVisualizerConstIterator Iterator = FeatureContainer.GetFeatureVisualizerConstIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterface_SelectableObject& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_SelectableObject>();
        Interface.Render(Object, _World);
    }
}