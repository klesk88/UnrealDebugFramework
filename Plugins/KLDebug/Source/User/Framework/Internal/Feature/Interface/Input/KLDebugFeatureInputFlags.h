// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/StaticBitArray.h"
#include "HAL/Platform.h"

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureInputFlags
{
public:
    enum class KLDEBUGUSERFRAMEWORK_API EFeatureUpdateFlags : uint8
    {
        SceneProxy = 0,

        Count
    };

private:
    using InputFeatureUpdateFlag = TStaticBitArray<static_cast<uint32>(EFeatureUpdateFlags::Count)>;

public:
    void SetFag(const EFeatureUpdateFlags _Flag);
    void ClearFag(const EFeatureUpdateFlags _Flag);
    [[nodiscard]] bool IsFlagSet(const EFeatureUpdateFlags _Flag) const;

private:
    InputFeatureUpdateFlag mFlags;
};

inline void FKLDebugFeatureInputFlags::SetFag(const EFeatureUpdateFlags _Flag)
{
    mFlags[static_cast<int32>(_Flag)] = true;
}

inline void FKLDebugFeatureInputFlags::ClearFag(const EFeatureUpdateFlags _Flag)
{
    mFlags[static_cast<int32>(_Flag)] = false;
}

inline bool FKLDebugFeatureInputFlags::IsFlagSet(const EFeatureUpdateFlags _Flag) const
{
    return mFlags[static_cast<int32>(_Flag)];
}