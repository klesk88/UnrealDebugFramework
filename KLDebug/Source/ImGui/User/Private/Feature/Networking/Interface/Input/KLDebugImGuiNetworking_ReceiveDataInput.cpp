#include "Feature/Networking/Input/KLDebugImGuiNetworking_ReceiveDataInput.h"

FKLDebugImGuiNetworking_ReceiveDataInput::FKLDebugImGuiNetworking_ReceiveDataInput(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _FeatureContext, FArchive& _Archive)
    : mWorld(_World)
    , mContextData(_FeatureContext)
    , mArchive(_Archive)
{
}
