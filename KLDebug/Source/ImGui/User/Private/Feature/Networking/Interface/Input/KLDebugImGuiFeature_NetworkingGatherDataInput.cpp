#include "Feature/Networking/Input/KLDebugImGuiFeature_NetworkingGatherDataInput.h"

FKLDebugImGuiFeature_NetworkingGatherDataInput::FKLDebugImGuiFeature_NetworkingGatherDataInput(const UWorld& _World, const EKLDebugImGuiNetworkingEnviroment _Enviroment, UObject& _OwnerObject, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive)
    : mOwnerObject(_OwnerObject)
    , mWorld(_World)
    , mContextData(_ContextData)
    , mArchive(_Archive)
    , mCurrentEnviroment(_Enviroment)
{
}
