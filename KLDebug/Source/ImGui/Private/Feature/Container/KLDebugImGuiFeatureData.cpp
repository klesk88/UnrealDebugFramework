#include "Feature/Container/KLDebugImGuiFeatureData.h"

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"

// engine
#include "Containers/UnrealString.h"

FKLDebugImGuiFeatureData::FKLDebugImGuiFeatureData(const KL::Debug::ImGui::Features::Types::FeatureOffset _FeatureOffset)
    : mFeaturesOffset(_FeatureOffset)
{
}

void FKLDebugImGuiFeatureData::Init(const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, TArray<FString>& _PathString)
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
