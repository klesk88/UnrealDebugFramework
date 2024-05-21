// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

// c++
#include <type_traits>

#define KL_DEBUG_RTTI(ItemType, ParentItemType)                                                       \
public:                                                                                               \
    [[nodiscard]] inline static const FName& StaticItemType()                                          \
    {                                                                                                 \
        static const FName Interface(TEXT(#ItemType));                                                \
        return Interface;                                                                             \
    }                                                                                                 \
                                                                                                      \
    [[nodiscard]] inline constexpr static bool IsDerivedFromParent()                                   \
    {                                                                                                 \
        return TIsDerivedFrom<ItemType, ParentItemType>::IsDerived;                                   \
    }                                                                                                 \
                                                                                                      \
    [[nodiscard]] inline const FName& GetID() const override                                           \
    {                                                                                                 \
        return StaticItemType();                                                                      \
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
 * Base class that allow for a basic RTTI system without going trough the UCLASS/USTRUCT
 * The user need to implement KL_DEBUG_RTTI macro on the header of the class.
 * For example
 *
 * class FMyClass final : public IKLDebugRTTIInterface
 * {
 *   ...KL_DEBUG_RTTI(FMyClass, IKLDebugRTTIInterface)
 * };
 *
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugRTTIInterface
{
public:
    virtual ~IKLDebugRTTIInterface();

    [[nodiscard]] virtual const FName& GetID() const = 0;

    template <typename OtherType>
    [[nodiscard]] bool IsDerivedFrom() const;

protected:
    // use the KL_DEBUG_RTTI macro for these
    [[nodiscard]] virtual bool IsDerivedInternal(const FName& _ItemTypeName) const;
    [[nodiscard]] static const FName& StaticItemType();
    // KL_DEBUG_RTTI
};

template <typename OtherType>
inline bool IKLDebugRTTIInterface::IsDerivedFrom() const
{
    return IsDerivedInternal(OtherType::StaticItemType());
}

inline bool IKLDebugRTTIInterface::IsDerivedInternal(const FName& _ItemTypeName) const
{
    return _ItemTypeName == StaticItemType();
}

inline const FName& IKLDebugRTTIInterface::StaticItemType()
{
    static const FName Interface(TEXT("IKLDebugRTTIInterface"));
    return Interface;
}