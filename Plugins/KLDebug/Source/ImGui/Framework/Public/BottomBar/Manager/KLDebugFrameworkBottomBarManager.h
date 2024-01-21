// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "BottomBar/Manager/KLDebugFrameworkBottomBarSortedData.h"
#include "Context/KLDebugContextInterface.h"

// modules
#include "User/Framework/Internal/StaticMemory/KLDebugUtilsStaticMemoryFactory.h"

// engine
#include "Templates/UniquePtr.h"

class IKLDebugBottomBarInterface;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugFrameworkBottomBarManager final : public TKLDebugUtilsStaticMemoryFactory<IKLDebugBottomBarInterface>
{
public:
    void Initialize();
    void Shutdown();

    void DrawBottomBar(const int32 _CurrentBottomBar, const UWorld& _World, IKLDebugContextInterface* _Context) const;

    UE_NODISCARD const TArray<FKLDebugFrameworkBottomBarSortedData>& GetSortedBars() const;
    UE_NODISCARD TUniquePtr<IKLDebugContextInterface> UpdateBottomBarIfNeeded(const UWorld& _World, const int32 _PrevIndex, const int32 _NewIndex);
    UE_NODISCARD const IKLDebugBottomBarInterface* TryGetCurrentInterface(const int32 _Index) const;
    UE_NODISCARD IKLDebugBottomBarInterface* TryGetCurrentInterfaceMutable(const int32 _Index);

private:
    TArray<FKLDebugFrameworkBottomBarSortedData> mSortedBars;
};

inline const TArray<FKLDebugFrameworkBottomBarSortedData>& FKLDebugFrameworkBottomBarManager::GetSortedBars() const
{
    return mSortedBars;
}
