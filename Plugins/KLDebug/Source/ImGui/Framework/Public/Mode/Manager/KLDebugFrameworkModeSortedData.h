// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/UnrealString.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class IKLDebugModeInterface;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugFrameworkModeSortedData final : public FNoncopyable
{
public:
    explicit FKLDebugFrameworkModeSortedData(const FString& _Name, IKLDebugModeInterface& _Interface);

    [[nodiscard]] const FString& GetName() const;
    [[nodiscard]] const IKLDebugModeInterface& GetInterface() const;
    [[nodiscard]] IKLDebugModeInterface& GetInterfaceMutable() const;

private:
    const FString& mName;
    IKLDebugModeInterface* mInterface = nullptr;
};

inline const FString& FKLDebugFrameworkModeSortedData::GetName() const
{
    return mName;
}

inline const IKLDebugModeInterface& FKLDebugFrameworkModeSortedData::GetInterface() const
{
    return GetInterfaceMutable();
}

inline IKLDebugModeInterface& FKLDebugFrameworkModeSortedData::GetInterfaceMutable() const
{
    checkf(mInterface != nullptr, TEXT("must be valid"));
    return *mInterface;
}