// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Container/KLDebugImGuiFeatureData.h"

// modules
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceBase.h"

// engine
#include "Containers/UnrealString.h"

FKLDebugImGuiFeatureData::FKLDebugImGuiFeatureData(const KL::Debug::ImGui::Features::Types::FeatureOffset _FeatureOffset)
    : mFeaturesOffset(_FeatureOffset)
{
}

void FKLDebugImGuiFeatureData::Init(const IKLDebugFeatureInterfaceBase& _FeatureInterface, TArray<FString>& _PathString)
{
    _PathString.Reset();
    const FString ImGuiPathAsString = _FeatureInterface.GetImGuiPath().ToString();
    ImGuiPathAsString.ParseIntoArray(_PathString, TEXT("."));
    mImGuiPathTokens.Reserve(_PathString.Num());

    for (const FString& Path : _PathString)
    {
        mImGuiPathTokens.Emplace(Path);
    }
}
