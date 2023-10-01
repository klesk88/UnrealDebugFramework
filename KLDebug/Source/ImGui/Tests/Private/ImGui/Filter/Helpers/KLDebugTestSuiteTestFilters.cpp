#include "ImGui/Filter/Helpers/KLDebugTestSuiteTestFilters.h"

#define KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(_ClassType)           \
    bool _ClassType::IsObjectValid(const UObject& _Object) const \
    {                                                            \
        return true;                                             \
    }

KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterOne)
KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterTwo)
KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterThree)
KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterFour)
KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterFive)
KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterSix)
KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterSeven)
KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterHeight)
KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterNine)
KLDEBUG_TESTSUITE_IMPLEMENT_FILTER(KLDebugImGuiTestFilterTen)

#undef KLDEBUG_TESTSUITE_IMPLEMENT_FILTER
