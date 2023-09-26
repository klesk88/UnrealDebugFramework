#include "Interface/Input/KLDebugImGuiNetworking_GatherDataInput.h"

FKLDebugImGuiNetworking_GatherDataInput::FKLDebugImGuiNetworking_GatherDataInput(const UWorld& _World, const EKLDebugImGuiNetworkingEnviroment _Enviroment, UObject& _OwnerObject, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive)
    : mOwnerObject(_OwnerObject)
    , mWorld(_World)
    , mContextData(_ContextData)
    , mArchive(_Archive)
    , mCurrentEnviroment(_Enviroment)
{
}
