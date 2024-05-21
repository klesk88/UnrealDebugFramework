// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Selectable.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Unique.h"

// modules
#include "User/Framework/Public/Context/KLDebugContextInterface.h"
#include "User/Framework/Public/Mode/KLDebugModeInterface.h"
#include "User/Framework/Public/Window/BottomBar/KLDebugBottomBarInterface.h"

// engine
#include "CoreMinimal.h"
#include "Engine/World.h"

class IKLDebugBottomBarInterface;
class IKLDebugModeInterface;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeaturesTickInput
{
public:
    explicit FKLDebugImGuiFeaturesTickInput(const UWorld& _World, const int32 _CurrentModeIndex, const int32 _BottomBarIndex, FKLDebugImGuiFeatureVisualizer_Unique& _UniqueFeatures, TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& _ObjectFeatures, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _Flags, IKLDebugModeInterface* _CurrentMode, IKLDebugContextInterface* _CurrentModeContext, IKLDebugBottomBarInterface* _CurrentBottomBar, IKLDebugContextInterface* _BottomBarContext);

    [[nodiscard]] const UWorld& GetOwnerWorld() const;
    [[nodiscard]] FKLDebugImGuiFeatureVisualizer_Unique& GetUniqueFeaturesMutable() const;
    [[nodiscard]] TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& GetObjectFeaturesMutable() const;
    [[nodiscard]] FKLDebugImGuiFeaturesTypesContainerManager& GetContainerManagerMutable() const;
    [[nodiscard]] KL::Debug::ImGui::Features::Types::FeatureEnableSet& GetFeatureFlagsMutable() const;
    [[nodiscard]] int32 GetCurrentModeIndex() const;
    [[nodiscard]] IKLDebugModeInterface* TryGetCurrentMode() const;
    [[nodiscard]] IKLDebugContextInterface* TryGetCurrentModeContext() const;
    [[nodiscard]] int32 GetBottomBarIndex() const;
    [[nodiscard]] IKLDebugBottomBarInterface* TryGetCurrentBottomBar() const;
    [[nodiscard]] IKLDebugContextInterface* TryGetCurrentBottomBarContext() const;

private:
    const UWorld& mWorld;
    FKLDebugImGuiFeatureVisualizer_Unique& mUniqueFeatures;
    TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& mObjectFeatures;
    FKLDebugImGuiFeaturesTypesContainerManager& mContainerManager;
    KL::Debug::ImGui::Features::Types::FeatureEnableSet& mFlags;
    int32 mCurrentModeIndex = -1;
    IKLDebugModeInterface* mCurrentMode = nullptr;
    IKLDebugContextInterface* mCurrentModeContext = nullptr;
    int32 mBottomBarIndex = -1;
    IKLDebugBottomBarInterface* mCurrentBottomBar = nullptr;
    IKLDebugContextInterface* mBottomBarContext = nullptr;
};

inline const UWorld& FKLDebugImGuiFeaturesTickInput::GetOwnerWorld() const
{
    return mWorld;
}

inline FKLDebugImGuiFeatureVisualizer_Unique& FKLDebugImGuiFeaturesTickInput::GetUniqueFeaturesMutable() const
{
    return mUniqueFeatures;
}

inline TArray<FKLDebugImGuiFeatureVisualizer_Selectable>& FKLDebugImGuiFeaturesTickInput::GetObjectFeaturesMutable() const
{
    return mObjectFeatures;
}

inline FKLDebugImGuiFeaturesTypesContainerManager& FKLDebugImGuiFeaturesTickInput::GetContainerManagerMutable() const
{
    return mContainerManager;
}

inline KL::Debug::ImGui::Features::Types::FeatureEnableSet& FKLDebugImGuiFeaturesTickInput::GetFeatureFlagsMutable() const
{
    return mFlags;
}

inline int32 FKLDebugImGuiFeaturesTickInput::GetCurrentModeIndex() const
{
    return mCurrentModeIndex;
}

inline IKLDebugModeInterface* FKLDebugImGuiFeaturesTickInput::TryGetCurrentMode() const
{
    return mCurrentMode;
}

inline IKLDebugContextInterface* FKLDebugImGuiFeaturesTickInput::TryGetCurrentModeContext() const
{
    return mCurrentModeContext;
}

inline int32 FKLDebugImGuiFeaturesTickInput::GetBottomBarIndex() const
{
    return mBottomBarIndex;
}

inline IKLDebugBottomBarInterface* FKLDebugImGuiFeaturesTickInput::TryGetCurrentBottomBar() const
{
    return mCurrentBottomBar;
}

inline IKLDebugContextInterface* FKLDebugImGuiFeaturesTickInput::TryGetCurrentBottomBarContext() const
{
    return mBottomBarContext;
}