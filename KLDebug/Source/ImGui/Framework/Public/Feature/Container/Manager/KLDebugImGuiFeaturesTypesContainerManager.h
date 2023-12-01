// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureManager;
class FKLDebugImGuiFeatureContainerBase;
class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeaturesTypesContainerManager final : public FNoncopyable
{
public:
    FKLDebugImGuiFeaturesTypesContainerManager();
    ~FKLDebugImGuiFeaturesTypesContainerManager();

    void Initialize();
    void Shutdown();

    UE_NODISCARD const FKLDebugImGuiFeatureContainerBase& GetContainer(const EImGuiInterfaceType _Type) const;
    UE_NODISCARD FKLDebugImGuiFeatureContainerBase& GetContainerMutable(const EImGuiInterfaceType _Type) const;

private:
    void CreateContainers();
    void GatherFeatures() const;

    void GetContainerAndOffset(const EImGuiInterfaceType _ContainerType, TArray<uint32>& ContainersOffset, FKLDebugImGuiFeatureContainerBase*& _OutContainer, uint32*& _OutOffset);

private:
    TArray<TUniquePtr<FKLDebugImGuiFeatureContainerBase>> mContainers;
};

inline const FKLDebugImGuiFeatureContainerBase& FKLDebugImGuiFeaturesTypesContainerManager::GetContainer(const EImGuiInterfaceType _Type) const
{
    return GetContainerMutable(_Type);
}
