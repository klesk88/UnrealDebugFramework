#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterfaceMacros.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "UObject/NameTypes.h"

class UWorld;
class IKLDebugImGuiFilterInterface;

#define DERIVED_KL_DEBUG_FEATURE_CLASS(ItemType, ParentItemType)                                      \
public:                                                                                               \
    UE_NODISCARD inline static const FName& StaticItemType()                                          \
    {                                                                                                 \
        static const FName Interface(TEXT(#ItemType));                                                \
        return Interface;                                                                             \
    }                                                                                                 \
                                                                                                      \
protected:                                                                                            \
    UE_NODISCARD inline bool IsDerivedInternal(const FName& _ItemTypeName) const override             \
    {                                                                                                 \
        return StaticItemType() == _ItemTypeName || ParentItemType::IsDerivedInternal(_ItemTypeName); \
    }                                                                                                 \
                                                                                                      \
private:  // finish macro that allow us to have a basic RTTI system

/*
 * Base class for any debug window to extend.
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_WINDOW() in the .cpp so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassDebug final : public IKLDebugImGuiFeatureInterface
 * {
 *   ...
 * };
 *
 * FClassDebug.cpp
 *
 * KL_DEBUG_CREATE_WINDOW(FClassDebug)
 */
class KLDEBUGIMGUI_API IKLDebugImGuiFeatureInterfaceBase
{
public:
    virtual ~IKLDebugImGuiFeatureInterfaceBase() = default;

    // get path that will be displayed inside the ImGui tree
    UE_NODISCARD virtual const FName& GetImGuiPath() const = 0;

    // this is called once at game startup from the engine module to initialize the class
    virtual void Initialize();
    virtual void Update();

    virtual void DrawImGui(const UWorld& _World);
    virtual void Render(const UWorld& _World) const;

    template<typename FeatureType>
    UE_NODISCARD bool IsDerivedFrom() const;

protected:
    virtual void           DrawImGuiChild(const UWorld& _World) = 0;
    virtual const FString& GetWindowName() const                = 0;

protected:
    // implement the DERIVED_KL_DEBUG_FEATURE_CLASS macro for these
    UE_NODISCARD virtual bool        IsDerivedInternal(const FName& _ItemTypeName) const;
    UE_NODISCARD static const FName& StaticItemType();
    //
};

inline void IKLDebugImGuiFeatureInterfaceBase::Initialize()
{
}

inline void IKLDebugImGuiFeatureInterfaceBase::Update()
{
}

inline void IKLDebugImGuiFeatureInterfaceBase::Render(const UWorld& _World) const
{
}

template<typename FeatureType>
inline bool IKLDebugImGuiFeatureInterfaceBase::IsDerivedFrom() const
{
    return IsDerivedInternal(FeatureType::StaticItemType());
}

inline bool IKLDebugImGuiFeatureInterfaceBase::IsDerivedInternal(const FName& _ItemTypeName) const
{
    return _ItemTypeName == StaticItemType();
}

inline const FName& IKLDebugImGuiFeatureInterfaceBase::StaticItemType()
{
    static const FName Interface(TEXT("ImGuiFeatureInterfaceBase"));
    return Interface;
}