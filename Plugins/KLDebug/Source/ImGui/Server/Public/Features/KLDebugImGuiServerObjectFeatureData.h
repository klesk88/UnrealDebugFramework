// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugContextGetterInput_Selectable;
class IKLDebugContextInterface;
class IKLDebugFeatureInterfaceBase;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerObjectFeatureData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiServerObjectFeatureData(const FKLDebugContextGetterInput_Selectable& _Input, const IKLDebugFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex);
    ~FKLDebugImGuiServerObjectFeatureData();
    UE_NODISCARD bool operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex) const;

    void SetLastSendCRC(const uint32 _CRC);

    UE_NODISCARD uint32 GetLastSentCRC() const;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetServerFeatureIndex() const;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetClientFeatureIndex() const;
    UE_NODISCARD IKLDebugContextInterface* GetContextMutable() const;
    UE_NODISCARD const IKLDebugContextInterface* GetContext() const;

private:
    TUniquePtr<IKLDebugContextInterface> mContext;
    // server and  client can be out of sync in terms of features indexes. So store both of the separately
    KL::Debug::ImGui::Features::Types::FeatureIndex mServerFeatureIndex = KL::Debug::ImGui::Features::Types::InvalidFeatureIndex;
    // the client will be used for when we write back to it
    KL::Debug::ImGui::Features::Types::FeatureIndex mClientFeatureIndex = KL::Debug::ImGui::Features::Types::InvalidFeatureIndex;
    uint32 mLastSentCRC = 0;
};

inline bool FKLDebugImGuiServerObjectFeatureData::operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex) const
{
    return mServerFeatureIndex == mServerFeatureIndex;
}

inline void FKLDebugImGuiServerObjectFeatureData::SetLastSendCRC(const uint32 _CRC)
{
    mLastSentCRC = _CRC;
}

inline uint32 FKLDebugImGuiServerObjectFeatureData::GetLastSentCRC() const
{
    return mLastSentCRC;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiServerObjectFeatureData::GetServerFeatureIndex() const
{
    return mServerFeatureIndex;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiServerObjectFeatureData::GetClientFeatureIndex() const
{
    return mClientFeatureIndex;
}

inline const IKLDebugContextInterface* FKLDebugImGuiServerObjectFeatureData::GetContext() const
{
    return GetContextMutable();
}