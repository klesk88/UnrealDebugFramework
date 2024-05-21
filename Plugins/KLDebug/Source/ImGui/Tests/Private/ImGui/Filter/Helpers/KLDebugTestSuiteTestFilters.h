// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// imgui user module
#include "User/Framework/Public/Filter/Interface/KLDebugFilterInterface.h"

class KLDebugImGuiTestFilterOne final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestOne)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterTwo final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestTwo)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterThree final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestThree)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterFour final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestFour)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterFive final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestFive)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterSix final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestSix)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterSeven final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestSeven)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterHeight final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestHeight)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterNine final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestNine)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};

///////////////////////////////////////////////////////

class KLDebugImGuiTestFilterTen final : public IKLDebugFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(TestTen)

public:
    [[nodiscard]] bool IsObjectValid(const UObject& _Object) const final;
};