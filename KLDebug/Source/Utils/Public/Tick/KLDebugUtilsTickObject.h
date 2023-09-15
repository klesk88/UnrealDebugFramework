#pragma once

// engine
#include "Engine/EngineBaseTypes.h"
#include "UObject/Class.h"

#include "KLDebugUtilsTickObject.generated.h"

DECLARE_DELEGATE_OneParam(FOnTick, const float /*_DeltaTime*/)

USTRUCT()
struct KLDEBUGUTILS_API FKLDebugUtilsTickObject final : public FTickFunction
{
    GENERATED_BODY()

public:
    //FTickFunction
    void ExecuteTick(float _DeltaTime, ELevelTick _TickType, ENamedThreads::Type _CurrentThread, const FGraphEventRef& _CompletionGraphEvent) final;
    virtual FString DiagnosticMessage() final;
    virtual FName DiagnosticContext(bool _Detailed) final;
    //FTickFunction

    void BindDelegate(const FOnTick& _Delegate);
    void ClearDelegate();

private:
    FOnTick mOnTick;
};

inline void FKLDebugUtilsTickObject::BindDelegate(const FOnTick& _Delegate)
{
    ensureMsgf(!mOnTick.IsBound(), TEXT("Already bound"));
    mOnTick = _Delegate;
}

inline void FKLDebugUtilsTickObject::ClearDelegate()
{
    mOnTick.Unbind();
}

inline FString FKLDebugUtilsTickObject::DiagnosticMessage()
{
    return TEXT("KLDebugImGuiNetwork");
}

inline FName FKLDebugUtilsTickObject::DiagnosticContext(bool _Detailed)
{
    return FName(TEXT("KLDebugImGuiNetwork"));
}

template<>
struct TStructOpsTypeTraits<FKLDebugUtilsTickObject> final : public TStructOpsTypeTraitsBase2<FKLDebugUtilsTickObject>
{
    enum
    {
        WithCopy = false
    };
};
