#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"

#include "Feature/Container/EngineSubsystem/KLDebugImGuiFeatureContainer_EngineSubsystem.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/ObjectSubsystem/KLDebugImGuiFeatureContainer_ObjectSubsystem.h"
#include "Feature/Container/SelectableObject/KLDebugImGuiFeatureContainer_SelectableObject.h"
#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Manager/KLDebugImGuiFeatureManager.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

// debug utils module
#include "Utils/Public/KLDebugLog.h"

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

FKLDebugImGuiFeatureContainerBase& FKLDebugImGuiFeaturesTypesContainerManager::GetContainerMutable(const EContainerType _Type) const
{
    check(_Type != EContainerType::COUNT);

    const TUniquePtr<FKLDebugImGuiFeatureContainerBase>& Container = mContainers[static_cast<int32>(_Type)];
    return *Container.Get();
}

void FKLDebugImGuiFeaturesTypesContainerManager::GetContainerAndOffset(const EContainerType _ContainerType, TArray<uint32>& ContainersOffset, FKLDebugImGuiFeatureContainerBase*& _OutContainer, uint32*& _OutOffset)
{
    _OutContainer = mContainers[static_cast<int32>(_ContainerType)].Get();
    _OutOffset    = &ContainersOffset[static_cast<int32>(_ContainerType)];
}

void FKLDebugImGuiFeaturesTypesContainerManager::CreateContainers()
{
    mContainers.Reserve(static_cast<int32>(EContainerType::COUNT));

    for (int32 i = 0; i < static_cast<int32>(EContainerType::COUNT); ++i)
    {
        switch (static_cast<EContainerType>(i))
        {
            case EContainerType::SELECTABLE_OBJECTS:
                mContainers.Emplace(MakeUnique<FKLDebugImGuiFeatureContainer_SelectableObject>());
                break;
            case EContainerType::ENGINE_SUBSYTSTEM:
                mContainers.Emplace(MakeUnique<FKLDebugImGuiFeatureContainer_EngineSubsystem>());
                break;
            case EContainerType::EDITOR_SUBSYSTEM:
                mContainers.Emplace(MakeUnique<FKLDebugImGuiFeatureContainer_ObjectSubsystem>(EObjectSubsytemType::EDITOR));
                break;
            case EContainerType::GAME_INSTANCE_SUBSYSTEM:
                mContainers.Emplace(MakeUnique<FKLDebugImGuiFeatureContainer_ObjectSubsystem>(EObjectSubsytemType::GAME_INSTANCE));
                break;
            case EContainerType::LOCAL_PLAYER_SUBSYSTEM:
                mContainers.Emplace(MakeUnique<FKLDebugImGuiFeatureContainer_ObjectSubsystem>(EObjectSubsytemType::LOCAL_PLAYER));
                break;
            case EContainerType::WORLD_SUBSYSTEM:
                mContainers.Emplace(MakeUnique<FKLDebugImGuiFeatureContainer_ObjectSubsystem>(EObjectSubsytemType::WORLD));
                break;
            default:
                checkNoEntry();
                break;
        }
    }
}

void FKLDebugImGuiFeaturesTypesContainerManager::GatherFeatures() const
{
    TArray<uint32> ContainersOffset;
    ContainersOffset.AddZeroed(mContainers.Num());

    const FKLDebugImGuiFeatureManager& FeatureManager = FKLDebugImGuiFeatureManager::Get();

    TArray<uint8> TempAllocatedFeature;
    TempAllocatedFeature.AddZeroed(FeatureManager.GetLargestEntrySize());

    for (const TUniquePtr<FKLDebugImGuiFeatureContainerBase>& FeatureContainer : mContainers)
    {
        FeatureContainer->InitGenerateFeatures(FeatureManager.GetTotalSizeRequired(), FeatureManager.GetEntryCount());
    }

    FKLDebugImGuiFeatureManagerEntryBase* Entry = FeatureManager.GetStartEntry();

    while (Entry)
    {
        FKLDebugImGuiFeatureContainerBase*       Container      = nullptr;
        uint32*                                  OffsetIndex    = nullptr;
        const IKLDebugImGuiFeatureInterfaceBase& TempWindowBase = Entry->AllocateInPlace(static_cast<void*>(&TempAllocatedFeature[0]));

        for (int32 i = 0; i < mContainers.Num(); ++i)
        {
            const TUniquePtr<FKLDebugImGuiFeatureContainerBase>& FeatureContainer = mContainers[i];
            if (FeatureContainer->IsCorrectContainerForFeature(TempWindowBase))
            {
                Container   = FeatureContainer.Get();
                OffsetIndex = &ContainersOffset[i];
                break;
            }
        }

        check(Container != nullptr && OffsetIndex != nullptr);

        Container->AllocateNewEntry(*Entry, *OffsetIndex);
        (*OffsetIndex) += Entry->GetSize();
        Entry = Entry->GetNextEntry();
    }

    for (const TUniquePtr<FKLDebugImGuiFeatureContainerBase>& FeatureContainer : mContainers)
    {
        FeatureContainer->FinishGenerateFeatures();
    }
}
