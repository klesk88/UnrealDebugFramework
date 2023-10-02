#include "Features/KLDebugImGuiServerObjectContainerFeatures.h"

FKLDebugImGuiServerObjectContainerFeatures::FKLDebugImGuiServerObjectContainerFeatures(const EImGuiInterfaceType _ContainerType)
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
