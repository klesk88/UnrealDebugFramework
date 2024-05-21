// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/UnrealString.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class IKLDebugBottomBarInterface;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugFrameworkBottomBarSortedData final : public FNoncopyable
{
public:
    explicit FKLDebugFrameworkBottomBarSortedData(const FString& _Name, IKLDebugBottomBarInterface& _Interface);

    [[nodiscard]] const FString& GetName() const;
    [[nodiscard]] const IKLDebugBottomBarInterface& GetInterface() const;
    [[nodiscard]] IKLDebugBottomBarInterface& GetInterfaceMutable() const;

private:
    const FString& mName;
    IKLDebugBottomBarInterface* mInterface;
};

inline const FString& FKLDebugFrameworkBottomBarSortedData::GetName() const
{
    return mName;
}

inline const IKLDebugBottomBarInterface& FKLDebugFrameworkBottomBarSortedData::GetInterface() const
{
    return GetInterfaceMutable();
}

inline IKLDebugBottomBarInterface& FKLDebugFrameworkBottomBarSortedData::GetInterfaceMutable() const
{
    checkf(mInterface != nullptr, TEXT("must be valid"));
    return *mInterface;
}