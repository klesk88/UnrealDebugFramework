// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"
#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryFactory.h"
#include "Mode/Manager/KLDebugFrameworkModeSortedData.h"

// engine
#include "Templates/UniquePtr.h"

class IKLDebugModeInterface;
class UCanvas;
class UFont;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugFrameworkModeManager final : public TKLDebugUtilsStaticMemoryFactory<IKLDebugModeInterface>
{
public:
    void Initialize();
    void Shutdown();

    void DrawImGui(const int32 _CurrentIndex, const UWorld& _World, IKLDebugContextInterface* _Context, bool& _RenderWindow) const;
    void DrawCanvas(const int32 _CurrentIndex, UWorld& _World, UCanvas& _Canvas, UFont& _Font, IKLDebugContextInterface* _Context) const;

    UE_NODISCARD const TArray<FKLDebugFrameworkModeSortedData>& GetSortedModes() const;
    UE_NODISCARD TUniquePtr<IKLDebugContextInterface> UpdateBottomBarIfNeeded(const UWorld& _World, const int32 _PrevIndex, const int32 _NewIndex);
    UE_NODISCARD bool RequireCanvasDraw(const int32 _CurrentIndex) const;
    UE_NODISCARD const IKLDebugModeInterface* TryGetCurrentInterface(const int32 _Index) const;
    UE_NODISCARD IKLDebugModeInterface* TryGetCurrentInterfaceMutable(const int32 _Index);

private:
    TArray<FKLDebugFrameworkModeSortedData> mSortedModes;
};

inline const TArray<FKLDebugFrameworkModeSortedData>& FKLDebugFrameworkModeManager::GetSortedModes() const
{
    return mSortedModes;
}
