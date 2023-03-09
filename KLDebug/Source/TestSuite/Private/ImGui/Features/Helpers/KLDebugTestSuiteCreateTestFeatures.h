#pragma once

// imgui module
#include "ImGui/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

class UObject;

class FKLDebugTestFeatureBase : public IKLDebugImGuiFeatureInterface_SelectableObject
{
public:
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    bool DoesSupportObject(const UObject& _Object) const final;
};

class FKLDebugTestCoverFeature final : public FKLDebugTestFeatureBase
{
public:
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverChildFeature final : public FKLDebugTestFeatureBase
{
public:
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverSecondChildFeature final : public FKLDebugTestFeatureBase
{
public:
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverThirdChildFeature final : public FKLDebugTestFeatureBase
{
public:
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverThirdChildChildFeature final : public FKLDebugTestFeatureBase
{
public:
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverChildChildFeature final : public FKLDebugTestFeatureBase
{
public:
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverChildChildChildFeature final : public FKLDebugTestFeatureBase
{
public:
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestPerceptionFeature final : public FKLDebugTestFeatureBase
{
public:
    const FName& GetImGuiPath() const final;
};