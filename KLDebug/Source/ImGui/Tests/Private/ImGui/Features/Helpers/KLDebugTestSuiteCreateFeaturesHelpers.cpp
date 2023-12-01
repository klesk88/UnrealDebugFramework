// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ImGui/Features/Helpers/KLDebugTestSuiteCreateFeaturesHelpers.h"

#include "ImGui/Features/Helpers/KLDebugTestSuiteTestFeatures.h"

// modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureData.h"

// engine
#include "Containers/UnrealString.h"

namespace KL::Debug::TestSuite::Feature::Helpers
{
    void GetFeatures(TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& _FeaturePool, TArray<FKLDebugImGuiFeatureData>& _FeatureData)
    {
        uint32 FullSize = 0;
        FullSize += sizeof(FKLDebugTestCoverFeature);
        FullSize += sizeof(FKLDebugTestCoverChildFeature);
        FullSize += sizeof(FKLDebugTestCoverSecondChildFeature);
        FullSize += sizeof(FKLDebugTestCoverThirdChildFeature);
        FullSize += sizeof(FKLDebugTestCoverThirdChildChildFeature);
        FullSize += sizeof(FKLDebugTestCoverChildChildFeature);
        FullSize += sizeof(FKLDebugTestCoverChildChildChildFeature);
        FullSize += sizeof(FKLDebugTestPerceptionFeature);

        _FeatureData.Reserve(8);
        _FeaturePool.AddZeroed(FullSize);

        TArray<FString> ImGuiPathTokens;
        ImGuiPathTokens.Reserve(20);

        KL::Debug::ImGui::Features::Types::FeatureOffset FeatureOffset = 0;
        for (int32 i = 0; i < 8; ++i)
        {
            uint32 Size = 0;
            IKLDebugImGuiFeatureInterfaceBase* DebugWindow = nullptr;
            FKLDebugImGuiFeatureData& NewFeatureData = _FeatureData.Emplace_GetRef(FeatureOffset);
            switch (i)
            {
            case 0:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&_FeaturePool[FeatureOffset])) FKLDebugTestCoverFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverFeature);
                break;
            case 1:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&_FeaturePool[FeatureOffset])) FKLDebugTestCoverChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverChildFeature);
                break;
            case 2:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&_FeaturePool[FeatureOffset])) FKLDebugTestCoverSecondChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverSecondChildFeature);
                break;
            case 3:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&_FeaturePool[FeatureOffset])) FKLDebugTestCoverThirdChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverThirdChildFeature);
                break;
            case 4:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&_FeaturePool[FeatureOffset])) FKLDebugTestCoverThirdChildChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverThirdChildChildFeature);
                break;
            case 5:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&_FeaturePool[FeatureOffset])) FKLDebugTestCoverChildChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverChildChildFeature);
                break;
            case 6:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&_FeaturePool[FeatureOffset])) FKLDebugTestCoverChildChildChildFeature());
                FeatureOffset += sizeof(FKLDebugTestCoverChildChildChildFeature);
                break;
            case 7:
                DebugWindow = reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(new (static_cast<void*>(&_FeaturePool[FeatureOffset])) FKLDebugTestPerceptionFeature());
                FeatureOffset += sizeof(FKLDebugTestPerceptionFeature);
                break;
            default:
                checkNoEntry();
                break;
            }

            NewFeatureData.Init(*DebugWindow, ImGuiPathTokens);
        }
    }
}    // namespace KL::Debug::TestSuite::Feature::Helpers