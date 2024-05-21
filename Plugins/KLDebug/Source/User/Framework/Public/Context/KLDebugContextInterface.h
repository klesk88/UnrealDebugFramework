// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

#define KL_DEBUG_DERIVED_CONTEXT(ItemType, ParentItemType)                                            \
public:                                                                                               \
    [[nodiscard]] inline static const FName& StaticItemType()                                          \
    {                                                                                                 \
        static const FName Interface(TEXT(#ItemType));                                                \
        return Interface;                                                                             \
    }                                                                                                 \
                                                                                                      \
protected:                                                                                            \
    [[nodiscard]] inline bool IsDerivedInternal(const FName& _ItemTypeName) const override             \
    {                                                                                                 \
        return StaticItemType() == _ItemTypeName || ParentItemType::IsDerivedInternal(_ItemTypeName); \
    }                                                                                                 \
                                                                                                      \
private:    // finish macro that allow us to have a basic RTTI system

/*
 * generic context that can be spawn by multiple classes to be able to have local data at runtime
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugContextInterface
{
public:
    virtual ~IKLDebugContextInterface();

    template <typename FeatureContextType>
    [[nodiscard]] bool IsDerivedFrom() const;

    template <typename FeatureContextType>
    [[nodiscard]] bool IsSameContext() const;

protected:
    // use the KL_DEBUG_DERIVED_CONTEXT macro for these
    [[nodiscard]] virtual bool IsDerivedInternal(const FName& _ItemTypeName) const;
    [[nodiscard]] static const FName& StaticItemType();
    //
};

template <typename FeatureContextType>
inline bool IKLDebugContextInterface::IsSameContext() const
{
    return FeatureContextType::StaticItemType() == StaticItemType();
}

template <typename FeatureContextType>
inline bool IKLDebugContextInterface::IsDerivedFrom() const
{
    return IsDerivedInternal(FeatureContextType::StaticItemType());
}

inline bool IKLDebugContextInterface::IsDerivedInternal(const FName& _ItemTypeName) const
{
    return _ItemTypeName == StaticItemType();
}

inline const FName& IKLDebugContextInterface::StaticItemType()
{
    static const FName Interface(TEXT("IKLDebugContext"));
    return Interface;
}
