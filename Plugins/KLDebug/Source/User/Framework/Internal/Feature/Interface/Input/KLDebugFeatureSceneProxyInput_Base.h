// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"

// engine
#include "DebugRenderSceneProxy.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class UPrimitiveComponent;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureSceneProxyInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugFeatureSceneProxyInput_Base(const UPrimitiveComponent& _RenderingComponent, IKLDebugContextInterface* _Context);
    virtual ~FKLDebugFeatureSceneProxyInput_Base() = default;

    [[nodiscard]] const UPrimitiveComponent& GetRenderingComponent() const;

    void SetDrawDelegateHelper(TUniquePtr<FDebugDrawDelegateHelper>&& _DelegateHelper);
    [[nodiscard]] TUniquePtr<FDebugDrawDelegateHelper>&& GetDrawDelegateHelper();

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    [[nodiscard]] ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    [[nodiscard]] const ContextType& GetContext() const;

private:
    const UPrimitiveComponent& mRenderingComponent;
    TUniquePtr<FDebugDrawDelegateHelper> mDrawDelegateHelper;
    IKLDebugContextInterface* mContext = nullptr;
};

inline const UPrimitiveComponent& FKLDebugFeatureSceneProxyInput_Base::GetRenderingComponent() const
{
    return mRenderingComponent;
}

inline void FKLDebugFeatureSceneProxyInput_Base::SetDrawDelegateHelper(TUniquePtr<FDebugDrawDelegateHelper>&& _DelegateHelper)
{
    ensureMsgf(!mDrawDelegateHelper.IsValid(), TEXT("setting multiple draw delegate helpers is not supported"));
    mDrawDelegateHelper = MoveTemp(_DelegateHelper);
}

inline TUniquePtr<FDebugDrawDelegateHelper>&& FKLDebugFeatureSceneProxyInput_Base::GetDrawDelegateHelper()
{
    return MoveTemp(mDrawDelegateHelper);
}

template <typename ContextType>
inline ContextType& FKLDebugFeatureSceneProxyInput_Base::GetContextMutable() const
{
    checkf(mContext && mContext->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContext);
}

template <typename ContextType>
inline const ContextType& FKLDebugFeatureSceneProxyInput_Base::GetContext() const
{
    return GetContextMutable<ContextType>();
}
