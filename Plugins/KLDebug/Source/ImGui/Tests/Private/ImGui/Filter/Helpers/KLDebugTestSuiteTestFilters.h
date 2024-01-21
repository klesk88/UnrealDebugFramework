// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// imgui user module
#include "User/Framework/Public/Filter/Interface/KLDebugFilterInterface.h"

class KLDebugImGuiTestFilterOne final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestOne)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterTwo final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestTwo)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterThree final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestThree)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterFour final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestFour)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterFive final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestFive)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterSix final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestSix)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterSeven final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestSeven)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterHeight final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestHeight)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterNine final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestNine)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterTen final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestTen)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};