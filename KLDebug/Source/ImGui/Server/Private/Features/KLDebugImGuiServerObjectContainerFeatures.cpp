#include "Features/KLDebugImGuiServerObjectContainerFeatures.h"

FKLDebugImGuiServerObjectContainerFeatures::FKLDebugImGuiServerObjectContainerFeatures(const EContainerType _ContainerType)
    : mContainerType(_ContainerType)
{
}

void FKLDebugImGuiServerObjectContainerFeatures::InitIfNeeded()
{
    if (mFeaturesEnable.Max() == 0)
    {
        mFeaturesEnable.Reserve(20);
    }
}
