// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Selectable/Input/KLDebugUserNetworkingFeatureSelectableRequestUpdateInput.h"

FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput::FKLDebugUserNetworkingFeatureSelectableRequestUpdateInput(const UWorld& _World, const UObject& _Object, FArchive& _ReaderArchive, FArchive& _WriterArchive)
    : FKLDebugUserNetworkingFeatureRequestUpdateInputBase(_World, _ReaderArchive, _WriterArchive)
    , mObject(_Object)
{
}
