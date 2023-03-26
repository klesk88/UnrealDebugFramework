#pragma once

// engine
#include "UObject/Interface.h"

#include "KLDebugImGuiSubsystemUpdatable.generated.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class UWorld;

UINTERFACE()
class KLDEBUGIMGUI_API UKLDebugImGuiSubsystemUpdatable : public UInterface
{
    GENERATED_BODY()
};

class KLDEBUGIMGUI_API IKLDebugImGuiSubsystemUpdatable
{
    GENERATED_BODY()

public:
    virtual void Initialize(FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)                             = 0;
    virtual void Update(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)    = 0;
    virtual void DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) = 0;
};