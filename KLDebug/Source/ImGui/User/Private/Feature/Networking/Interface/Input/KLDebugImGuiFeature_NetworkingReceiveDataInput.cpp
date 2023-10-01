#include "Feature/Networking/Input/KLDebugImGuiFeature_NetworkingReceiveDataInput.h"

FKLDebugImGuiFeature_NetworkingReceiveDataInput::FKLDebugImGuiFeature_NetworkingReceiveDataInput(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _FeatureContext, FArchive& _Archive)
    : mWorld(_World)
    , mContextData(_FeatureContext)
    , mArchive(_Archive)
{
}
