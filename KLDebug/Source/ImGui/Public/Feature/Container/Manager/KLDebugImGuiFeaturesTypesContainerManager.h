#pragma once

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureManager;
class FKLDebugImGuiFeatureContainerBase;
class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUI_API FKLDebugImGuiFeaturesTypesContainerManager final : public FNoncopyable
{
public:
    FKLDebugImGuiFeaturesTypesContainerManager();
    ~FKLDebugImGuiFeaturesTypesContainerManager();

    void Initialize();
    void Shutdown();

    UE_NODISCARD const FKLDebugImGuiFeatureContainerBase& GetContainer(const EContainerType _Type) const;
    UE_NODISCARD FKLDebugImGuiFeatureContainerBase&       GetContainerMutable(const EContainerType _Type) const;

private:
    void CreateContainers();
    void GatherFeatures() const;

    void GetContainerAndOffset(const EContainerType _ContainerType, TArray<uint32>& ContainersOffset, FKLDebugImGuiFeatureContainerBase*& _OutContainer, uint32*& _OutOffset);

private:
    TArray<TUniquePtr<FKLDebugImGuiFeatureContainerBase>> mContainers;
};

inline const FKLDebugImGuiFeatureContainerBase& FKLDebugImGuiFeaturesTypesContainerManager::GetContainer(const EContainerType _Type) const
{
    return GetContainerMutable(_Type);
}
