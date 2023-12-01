// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Engine/EngineBaseTypes.h"
#include "UObject/Class.h"

#include "KLDebugImGuiTickObject.generated.h"

DECLARE_DELEGATE_OneParam(FOnTick, const float /*_DeltaTime*/)

USTRUCT()
struct KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiTickObject final : public FTickFunction
{
    GENERATED_BODY()

public:
    // FTickFunction
    void ExecuteTick(float _DeltaTime, ELevelTick _TickType, ENamedThreads::Type _CurrentThread, const FGraphEventRef& _CompletionGraphEvent) final;
    virtual FString DiagnosticMessage() final;
    virtual FName DiagnosticContext(bool _Detailed) final;
    // FTickFunction

    void BindDelegate(const FOnTick& _Delegate);
    void ClearDelegate();

private:
    FOnTick mOnTick;
};

inline void FKLDebugImGuiTickObject::BindDelegate(const FOnTick& _Delegate)
{
    ensureMsgf(!mOnTick.IsBound(), TEXT("Already bound"));
    mOnTick = _Delegate;
}

inline void FKLDebugImGuiTickObject::ClearDelegate()
{
    mOnTick.Unbind();
}

inline FString FKLDebugImGuiTickObject::DiagnosticMessage()
{
    return TEXT("KLDebugImGuiNetwork");
}

inline FName FKLDebugImGuiTickObject::DiagnosticContext(bool _Detailed)
{
    return FName(TEXT("KLDebugImGuiNetwork"));
}

template <>
struct TStructOpsTypeTraits<FKLDebugImGuiTickObject> final : public TStructOpsTypeTraitsBase2<FKLDebugImGuiTickObject>
{
    enum
    {
        WithCopy = false
    };
};
