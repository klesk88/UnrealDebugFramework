// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// imgui user module
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"

class UObject;

class FKLDebugTestFeatureBase : public IKLDebugImGuiFeatureInterface_Selectable
{
private:
    void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Selectable& _Input) final;
    const FString& GetWindowName() const final;
};

class FKLDebugTestCoverFeature final : public FKLDebugTestFeatureBase
{
public:
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverChildFeature final : public FKLDebugTestFeatureBase
{
public:
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverSecondChildFeature final : public FKLDebugTestFeatureBase
{
public:
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverThirdChildFeature final : public FKLDebugTestFeatureBase
{
public:
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverThirdChildChildFeature final : public FKLDebugTestFeatureBase
{
public:
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverChildChildFeature final : public FKLDebugTestFeatureBase
{
public:
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestCoverChildChildChildFeature final : public FKLDebugTestFeatureBase
{
public:
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    const FName& GetImGuiPath() const final;
};

class FKLDebugTestPerceptionFeature final : public FKLDebugTestFeatureBase
{
public:
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    const FName& GetImGuiPath() const final;
};
