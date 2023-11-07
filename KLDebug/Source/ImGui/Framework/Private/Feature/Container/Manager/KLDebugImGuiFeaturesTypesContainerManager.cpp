#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Selectable/KLDebugImGuiFeatureContainer_Selectable.h"
#include "Feature/Container/Unique/KLDebugImGuiFeatureContainer_Unique.h"

//modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Internal/Feature/Manager/KLDebugImGuiFeatureManager.h"
#include "ImGui/User/Internal/Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Containers/UnrealString.h"
#include "Math/NumericLimits.h"
#include "UObject/NameTypes.h"

FKLDebugImGuiFeaturesTypesContainerManager::FKLDebugImGuiFeaturesTypesContainerManager()
{
}

FKLDebugImGuiFeaturesTypesContainerManager::~FKLDebugImGuiFeaturesTypesContainerManager()
{
}

void FKLDebugImGuiFeaturesTypesContainerManager::Initialize()
{
    CreateContainers();
    GatherFeatures();
}

void FKLDebugImGuiFeaturesTypesContainerManager::Shutdown()
{
    mContainers.Empty();
}

FKLDebugImGuiFeatureContainerBase& FKLDebugImGuiFeaturesTypesContainerManager::GetContainerMutable(const EImGuiInterfaceType _Type) const
{
    check(_Type != EImGuiInterfaceType::COUNT);

    const TUniquePtr<FKLDebugImGuiFeatureContainerBase>& Container = mContainers[static_cast<int32>(_Type)];
    return *Container.Get();
}

void FKLDebugImGuiFeaturesTypesContainerManager::GetContainerAndOffset(const EImGuiInterfaceType _ContainerType, TArray<uint32>& ContainersOffset, FKLDebugImGuiFeatureContainerBase*& _OutContainer, uint32*& _OutOffset)
{
    _OutContainer = mContainers[static_cast<int32>(_ContainerType)].Get();
    _OutOffset    = &ContainersOffset[static_cast<int32>(_ContainerType)];
}

void FKLDebugImGuiFeaturesTypesContainerManager::CreateContainers()
{
    mContainers.Reserve(static_cast<int32>(EImGuiInterfaceType::COUNT));

    for (int32 i = 0; i < static_cast<int32>(EImGuiInterfaceType::COUNT); ++i)
    {
        switch (static_cast<EImGuiInterfaceType>(i))
        {
            case EImGuiInterfaceType::SELECTABLE:
                mContainers.Emplace(MakeUnique<FKLDebugImGuiFeatureContainer_Selectable>());
                break;
            case EImGuiInterfaceType::UNIQUE:
                mContainers.Emplace(MakeUnique<FKLDebugImGuiFeatureContainer_Unique>());
                break;
            default:
                checkNoEntry();
                break;
        }
    }
}

void FKLDebugImGuiFeaturesTypesContainerManager::GatherFeatures() const
{
    TArray<KL::Debug::ImGui::Features::Types::FeatureOffset> ContainersOffset;
    ContainersOffset.AddZeroed(mContainers.Num());

    const FKLDebugImGuiFeatureManager& FeatureManager = FKLDebugImGuiFeatureManager::Get();

    for (const TUniquePtr<FKLDebugImGuiFeatureContainerBase>& FeatureContainer : mContainers)
    {
        FeatureContainer->InitGenerateFeatures(FeatureManager.GetTotalSizeRequired(), FeatureManager.GetEntryCount());
    }

    FKLDebugImGuiFeatureManagerEntryBase* Entry = FeatureManager.GetStartEntry();

    TArray<FString> ImGuiPathTokens;
    ImGuiPathTokens.Reserve(20);

#if DO_ENSURE
    TArray<FName, TInlineAllocator<100>> EnsureWindowNamesID;
#endif

    while (Entry)
    {
        const EImGuiInterfaceType InterfaceType = Entry->GetInterfaceType();
        checkf(InterfaceType != EImGuiInterfaceType::COUNT, TEXT("interface type must be valid"));

        const int32 ContainerIndex = static_cast<int32>(InterfaceType);

        checkf(mContainers.IsValidIndex(ContainerIndex) && mContainers[ContainerIndex]->GetContainerType() == InterfaceType, TEXT("wrong container picked"));
        checkf(ContainersOffset.IsValidIndex(ContainerIndex), TEXT("wrong container offset picked"));

        FKLDebugImGuiFeatureContainerBase& Container = *mContainers[ContainerIndex].Get();
        const KL::Debug::ImGui::Features::Types::FeatureOffset OffsetIndex = ContainersOffset[ContainerIndex];

        IKLDebugImGuiFeatureInterfaceBase& NewInterface = Container.AllocateNewEntry(*Entry, OffsetIndex, ImGuiPathTokens);
#if DO_ENSURE
        ensureMsgf(EnsureWindowNamesID.Find(NewInterface.GetFeatureNameID()) == INDEX_NONE, TEXT("we found a feature with the same imgui path. Please change the imgui path value"));
        EnsureWindowNamesID.Emplace(NewInterface.GetFeatureNameID());
#endif
        checkf(static_cast<uint64>(OffsetIndex) + static_cast<uint64>(Entry->GetSize()) < TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureOffset>::Max(), TEXT("feature offset data type is not big enough"));
        ContainersOffset[ContainerIndex] += Entry->GetSize();
        Entry = Entry->GetNextEntry();
    }

    for (const TUniquePtr<FKLDebugImGuiFeatureContainerBase>& FeatureContainer : mContainers)
    {
        FeatureContainer->FinishGenerateFeatures();
    }
}
