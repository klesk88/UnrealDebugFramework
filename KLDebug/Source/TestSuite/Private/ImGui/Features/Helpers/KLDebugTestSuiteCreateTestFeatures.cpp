#include "ImGui/Features/Helpers/KLDebugTestSuiteCreateTestFeatures.h"

void FKLDebugTestFeatureBase::GetFilterPath(TArray<FName>& _OutFilters) const
{
}

bool FKLDebugTestFeatureBase::DoesSupportObject(const UObject& _Object) const
{
    return true;
}

////////////////////////////////////////////////////////////////

const FName& FKLDebugTestCoverFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.1"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

const FName& FKLDebugTestCoverChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.1.a"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

const FName& FKLDebugTestCoverSecondChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.1.b"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

const FName& FKLDebugTestCoverThirdChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.2"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

const FName& FKLDebugTestCoverThirdChildChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.3.a"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

const FName& FKLDebugTestCoverChildChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.3.b"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

const FName& FKLDebugTestCoverChildChildChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.1.a.1"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

const FName& FKLDebugTestPerceptionFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.perception"));
    return FeatureName;
}
