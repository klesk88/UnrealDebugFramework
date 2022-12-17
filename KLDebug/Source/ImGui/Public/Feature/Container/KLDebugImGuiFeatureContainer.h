#pragma once

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"

class IKLDebugImGuiFeatureInterface;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureContainer final : public FNoncopyable
{
public:
    void Initialize();
    void Shutdown();

private:
    UE_NODISCARD IKLDebugImGuiFeatureInterface&       GetFeatureMutable(const uint32 _Offset);
    UE_NODISCARD const IKLDebugImGuiFeatureInterface& GetFeature(const uint32 _Offset) const;

    void GatherFeatures();
    void DestroyFeatures();

private:
    //offset between features so that we can retrieve them correctly from the pool. Each entry is the
    //start adress index of a new feature inside mFeaturesPool
    TArray<uint32> mFeaturesOffset;
    //pool of features. This is a byte array and all features are allocated inside of it
    //in this way they are all packed in memory close together
    TArray<uint8> mFeaturesPool;
};
