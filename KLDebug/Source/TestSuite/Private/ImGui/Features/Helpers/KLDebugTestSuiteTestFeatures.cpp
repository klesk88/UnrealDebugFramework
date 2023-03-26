#include "ImGui/Features/Helpers/KLDebugTestSuiteTestFeatures.h"

#include "ImGui/Filter/Helpers/KLDebugTestSuiteTestFilters.h"

bool FKLDebugTestFeatureBase::DoesSupportObject(const UObject& _Object) const
{
    return true;
}

void FKLDebugTestFeatureBase::DrawImGuiChild(const UWorld& _World)
{
}

const FString& FKLDebugTestFeatureBase::GetWindowName() const
{
    static FString EmptyName;
    return EmptyName;
}

////////////////////////////////////////////////////////////////

void FKLDebugTestCoverFeature::GetFilterPath(TArray<FName>& _OutFilters) const
{
    GetFilterPathHelper<KLDebugImGuiTestFilterOne, KLDebugImGuiTestFilterTwo>(_OutFilters);
}

const FName& FKLDebugTestCoverFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.1"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

void FKLDebugTestCoverChildFeature::GetFilterPath(TArray<FName>& _OutFilters) const
{
    GetFilterPathHelper<KLDebugImGuiTestFilterOne, KLDebugImGuiTestFilterTwo, KLDebugImGuiTestFilterThree, KLDebugImGuiTestFilterFour>(_OutFilters);
}

const FName& FKLDebugTestCoverChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.1.a"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

void FKLDebugTestCoverSecondChildFeature::GetFilterPath(TArray<FName>& _OutFilters) const
{
    GetFilterPathHelper<KLDebugImGuiTestFilterOne, KLDebugImGuiTestFilterTwo, KLDebugImGuiTestFilterFour, KLDebugImGuiTestFilterThree>(_OutFilters);
}

const FName& FKLDebugTestCoverSecondChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.1.b"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

void FKLDebugTestCoverThirdChildFeature::GetFilterPath(TArray<FName>& _OutFilters) const
{
    GetFilterPathHelper<KLDebugImGuiTestFilterOne, KLDebugImGuiTestFilterTwo, KLDebugImGuiTestFilterTen>(_OutFilters);
}

const FName& FKLDebugTestCoverThirdChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.2"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

void FKLDebugTestCoverThirdChildChildFeature::GetFilterPath(TArray<FName>& _OutFilters) const
{
    GetFilterPathHelper<KLDebugImGuiTestFilterNine>(_OutFilters);
}

const FName& FKLDebugTestCoverThirdChildChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.3.a"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

void FKLDebugTestCoverChildChildFeature::GetFilterPath(TArray<FName>& _OutFilters) const
{
    GetFilterPathHelper<KLDebugImGuiTestFilterSeven, KLDebugImGuiTestFilterHeight>(_OutFilters);
}

const FName& FKLDebugTestCoverChildChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.3.b"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

void FKLDebugTestCoverChildChildChildFeature::GetFilterPath(TArray<FName>& _OutFilters) const
{
    GetFilterPathHelper<KLDebugImGuiTestFilterOne, KLDebugImGuiTestFilterTwo>(_OutFilters);
}

const FName& FKLDebugTestCoverChildChildChildFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.cover.1.a.1"));
    return FeatureName;
}

////////////////////////////////////////////////////////////////

void FKLDebugTestPerceptionFeature::GetFilterPath(TArray<FName>& _OutFilters) const
{
}

const FName& FKLDebugTestPerceptionFeature::GetImGuiPath() const
{
    static const FName FeatureName(TEXT("ai.perception"));
    return FeatureName;
}
