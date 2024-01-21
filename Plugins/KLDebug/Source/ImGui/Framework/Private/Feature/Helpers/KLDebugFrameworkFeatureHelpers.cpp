#include "Feature/Helpers/KLDebugFrameworkFeatureHelpers.h"

// modules
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceBase.h"
#include "User/Framework/Public/Feature/Interface/Selectable/KLDebugFeatureInterface_Selectable.h"
#include "User/Framework/Public/Feature/Interface/Unique/KLDebugFeatureInterface_Unique.h"

namespace KL::Debug::Feature::Helpers
{
    /////////////////////////////////////////////////////////////////////////////////////
    /// public

    void OnFeatureUnselected(const UWorld& _World, UObject* _OwnerObject, IKLDebugFeatureInterfaceBase& _RemovedFeature)
    {
        if (_RemovedFeature.IsDerivedFrom<IKLDebugFeatureInterface_Unique>())
        {
            IKLDebugFeatureInterface_Unique& RemovedUniqueInterface = static_cast<IKLDebugFeatureInterface_Unique&>(_RemovedFeature);
            RemovedUniqueInterface.OnFeatureUnselected(_World);
        }
        else
        {
            IKLDebugFeatureInterface_Selectable& RemovedSelectableInterface = static_cast<IKLDebugFeatureInterface_Selectable&>(_RemovedFeature);
            if (_OwnerObject)
            {
                RemovedSelectableInterface.OnFeatureUnselected(*_OwnerObject);
            }
        }
    }

}    // namespace KL::Debug::Feature::Helpers