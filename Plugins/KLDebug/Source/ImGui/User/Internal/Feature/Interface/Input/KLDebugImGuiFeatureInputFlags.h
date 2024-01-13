// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/StaticBitArray.h"
#include "HAL/Platform.h"

class FKLDebugImGuiFeatureInputFlags
{
public:
    enum class KLDEBUGIMGUIUSER_API EFeatureUpdateFlags : uint8
    {
        SceneProxy = 0,

        Count
    };

private:
    using InputFeatureUpdateFlag = TStaticBitArray<static_cast<uint32>(EFeatureUpdateFlags::Count)>;

public:
    void SetFag(const EFeatureUpdateFlags _Flag);
    void ClearFag(const EFeatureUpdateFlags _Flag);
    UE_NODISCARD bool IsFlagSet(const EFeatureUpdateFlags _Flag) const;

private:
    InputFeatureUpdateFlag mFlags;
};

inline void FKLDebugImGuiFeatureInputFlags::SetFag(const EFeatureUpdateFlags _Flag)
{
    mFlags[static_cast<int32>(_Flag)] = true;
}

inline void FKLDebugImGuiFeatureInputFlags::ClearFag(const EFeatureUpdateFlags _Flag)
{
    mFlags[static_cast<int32>(_Flag)] = false;
}

inline bool FKLDebugImGuiFeatureInputFlags::IsFlagSet(const EFeatureUpdateFlags _Flag) const
{
    return mFlags[static_cast<int32>(_Flag)];
}