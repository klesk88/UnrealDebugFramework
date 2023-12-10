// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Networking/Input/KLDebugImGuiFeature_NetworkingGatherDataInput.h"

// engine
#include "GameFramework/Actor.h"

FKLDebugImGuiFeature_NetworkingGatherDataInput::FKLDebugImGuiFeature_NetworkingGatherDataInput(const UWorld& _World, UObject* _OwnerObject, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive)
    : mOwnerObject(_OwnerObject)
    , mWorld(_World)
    , mContextData(_ContextData)
    , mArchive(_Archive)
{
    if (!_OwnerObject)
    {
        return;
    }

    mHasAuthotityOnObject = true;
    if (const AActor* Actor = Cast<const AActor>(_OwnerObject))
    {
        mHasAuthotityOnObject = Actor->HasAuthority();
    }
}
