// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// modules
#include "User/Framework/Public/Context/KLDebugContextInterface.h"

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UniquePtr.h"

class FKLDebugContextGetterInput;
class IKLDebugFeatureInterfaceBase;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerUniqueFeatureData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiServerUniqueFeatureData(const FKLDebugContextGetterInput& _Input, const IKLDebugFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex);
    [[nodiscard]] bool operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex) const;

    [[nodiscard]] const IKLDebugContextInterface* GetContext() const;
    [[nodiscard]] IKLDebugContextInterface* GetFeatureContextMutable() const;
    [[nodiscard]] KL::Debug::ImGui::Features::Types::FeatureIndex GetServerFeatureIndex() const;
    [[nodiscard]] KL::Debug::ImGui::Features::Types::FeatureIndex GetClientFeatureIndex() const;
    [[nodiscard]] uint32 GetCRC() const;
    void SetCRC(const uint32 _CRC);

private:
    TUniquePtr<IKLDebugContextInterface> mContext;
    KL::Debug::ImGui::Features::Types::FeatureIndex mServerFeatureIndex = KL::Debug::ImGui::Features::Types::InvalidFeatureIndex;
    KL::Debug::ImGui::Features::Types::FeatureIndex mClientFeatureIndex = KL::Debug::ImGui::Features::Types::InvalidFeatureIndex;
    uint32 mCRC = 0;
};

inline bool FKLDebugImGuiServerUniqueFeatureData::operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex) const
{
    return mServerFeatureIndex == _ServerFeatureIndex;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiServerUniqueFeatureData::GetServerFeatureIndex() const
{
    return mServerFeatureIndex;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiServerUniqueFeatureData::GetClientFeatureIndex() const
{
    return mClientFeatureIndex;
}

inline uint32 FKLDebugImGuiServerUniqueFeatureData::GetCRC() const
{
    return mCRC;
}

inline void FKLDebugImGuiServerUniqueFeatureData::SetCRC(const uint32 _CRC)
{
    mCRC = _CRC;
}
