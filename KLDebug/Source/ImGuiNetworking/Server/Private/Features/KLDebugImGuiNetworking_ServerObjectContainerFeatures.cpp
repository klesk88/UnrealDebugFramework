#include "Features/KLDebugImGuiNetworking_ServerObjectContainerFeatures.h"

FKLDebugImGuiNetworking_ServerObjectContainerFeatures::FKLDebugImGuiNetworking_ServerObjectContainerFeatures(const EContainerType _ContainerType)
    : mContainerType(_ContainerType)
{
}

void FKLDebugImGuiNetworking_ServerObjectContainerFeatures::InitIfNeeded()
{
    if (mFeaturesEnable.Max() == 0)
    {
        mFeaturesEnable.Reserve(20);
    }
}
