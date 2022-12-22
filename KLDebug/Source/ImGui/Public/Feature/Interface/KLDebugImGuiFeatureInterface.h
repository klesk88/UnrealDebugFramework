#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterfaceMacros.h"

// engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
* Base class for any debug window to extend.
* IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_WINDOW() so that the class
* can auto subscribe to the system. For example:
* 
* class FClassDebug final : public IKLDebugImGuiFeatureInterface
* {
*   ...
* };
* 
* KL_DEBUG_CREATE_WINDOW(FClassDebug)
*/
class KLDEBUGIMGUI_API IKLDebugImGuiFeatureInterface
{
public:
    virtual ~IKLDebugImGuiFeatureInterface() = default;
    
    //get the tag associated with this feature.
    //IMPORTANT: the root node of the tags used must be KL.Debug
    UE_NODISCARD virtual const FGameplayTag& GetTag() const = 0;

    //this is called once at game startup from the engine module to initialize the class
    virtual void Initialize();
    virtual void Update();

    //must return true if the feature is debugging an entity which can be selected trough the picker
    //by default it returns true
    UE_NODISCARD virtual bool DoesSupportSelection() const;
    //should return true if the object passed as input is supported by this feature
    UE_NODISCARD virtual bool DoesSupportObject(const UObject& _Object) const;
};

inline void IKLDebugImGuiFeatureInterface::Initialize()
{
}

inline void IKLDebugImGuiFeatureInterface::Update()
{
}

inline bool IKLDebugImGuiFeatureInterface::DoesSupportSelection() const
{
    return true;
}

inline bool IKLDebugImGuiFeatureInterface::DoesSupportObject(const UObject& _Object) const
{
    return false;
}