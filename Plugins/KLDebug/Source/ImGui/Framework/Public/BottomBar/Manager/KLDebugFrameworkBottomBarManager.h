// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "BottomBar/Manager/KLDebugFrameworkBottomBarSortedData.h"

// modules
#include "User/Framework/Public/Context/KLDebugContextInterface.h"
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryFactory.h"

// engine
#include "Templates/UniquePtr.h"

class IKLDebugBottomBarInterface;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugFrameworkBottomBarManager final : public TKLDebugUtilsStaticMemoryFactory<IKLDebugBottomBarInterface>
{
public:
    void Initialize();
    void Shutdown();

    UE_NODISCARD const IKLDebugBottomBarInterface* TryGetBottomBar(const FName& _ID) const;

    UE_NODISCARD const TArray<FKLDebugFrameworkBottomBarSortedData>& GetSortedBars() const;
    void UpdateBottomBarIfNeeded(const UWorld& _World, const int32 _Index);

    void DrawBottomBar(const UWorld& _World) const;

private:
    TArray<FKLDebugFrameworkBottomBarSortedData> mSortedBars;
    IKLDebugBottomBarInterface* mCurrentSelectedBottomBar = nullptr;
    TUniquePtr<IKLDebugContextInterface> mCurrentContext;
    int32 mCurrentIndex = -1;
};

inline const IKLDebugBottomBarInterface* FKLDebugFrameworkBottomBarManager::TryGetBottomBar(const FName& _ID) const
{
    return TryGetData(_ID);
}

inline const TArray<FKLDebugFrameworkBottomBarSortedData>& FKLDebugFrameworkBottomBarManager::GetSortedBars() const
{
    return mSortedBars;
}
