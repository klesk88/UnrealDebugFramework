// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/Input/KLDebugContextGetterInput.h"

class UObject;
class UWorld;

/*
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugContextGetterInput_Selectable final : public FKLDebugContextGetterInput
{
public:
    explicit FKLDebugContextGetterInput_Selectable(const UObject& _Object, const bool _HasAuthority, const UWorld& _World, const ENetMode _CurrentNetMode);

    UE_NODISCARD const UObject& GetObject() const;
    UE_NODISCARD bool HasAuthorityOnObject() const;

private:
    const UObject& mObject;
    bool mHasAuthority = false;
};

inline const UObject& FKLDebugContextGetterInput_Selectable::GetObject() const
{
    return mObject;
}

inline bool FKLDebugContextGetterInput_Selectable::HasAuthorityOnObject() const
{
    return mHasAuthority;
}