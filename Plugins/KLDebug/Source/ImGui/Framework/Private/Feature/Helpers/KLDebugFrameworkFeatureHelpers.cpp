#include "Feature/Helpers/KLDebugFrameworkFeatureHelpers.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"

namespace KL::Debug::Feature::Helpers
{
    /////////////////////////////////////////////////////////////////////////////////////
    /// public

    void OnFeatureUnselected(const UWorld& _World, UObject* _OwnerObject, IKLDebugImGuiFeatureInterfaceBase& _RemovedFeature)
    {
        if (_RemovedFeature.IsDerivedFrom<IKLDebugImGuiFeatureInterface_Unique>())
        {
            IKLDebugImGuiFeatureInterface_Unique& RemovedUniqueInterface = static_cast<IKLDebugImGuiFeatureInterface_Unique&>(_RemovedFeature);
            RemovedUniqueInterface.OnFeatureUnselected(_World);
        }
        else
        {
            IKLDebugImGuiFeatureInterface_Selectable& RemovedSelectableInterface = static_cast<IKLDebugImGuiFeatureInterface_Selectable&>(_RemovedFeature);
            if (_OwnerObject)
            {
                RemovedSelectableInterface.OnFeatureUnselected(*_OwnerObject);
            }
        }
    }

}    // namespace KL::Debug::Feature::Helpers