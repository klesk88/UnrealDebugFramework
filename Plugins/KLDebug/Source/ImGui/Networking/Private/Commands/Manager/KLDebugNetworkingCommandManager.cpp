// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Commands/Manager/KLDebugNetworkingCommandManager.h"
#include "Log/KLDebugNetworkingLog.h"

// modules
#include "Networking/Runtime/Public/Commands/Factory/KLDebugNetworkingCommandFactory.h"
#include "User/Networking/Internal/Commands/Helpers/KLDebugUserNetworkingCommandsRequester.h"

// engine
#include "Containers/Array.h"
#include "Engine/World.h"

namespace KL::Debug::Networking::Commands::Manager
{
    /////////////////////////////////////////////////////
    /// private

    class KLDEBUGIMGUINETWORKING_API FCommandRequesterDelegate
    {
    public:
        explicit FCommandRequesterDelegate(const FObjectKey& _WorldKey, const FOnKLDebugNewCommandWorld& _Delegate);
        UE_NODISCARD bool operator==(const FObjectKey& _WorldKey) const;

        void Broadcast(const KL::Debug::Networking::Commands::Internal::FKLDebugOnNewCommandInput& _Input) const;

    private:
        FOnKLDebugNewCommandWorld mDelegate;
        FObjectKey mWorldKey;
    };

    FCommandRequesterDelegate::FCommandRequesterDelegate(const FObjectKey& _WorldKey, const FOnKLDebugNewCommandWorld& _Delegate)
        : mDelegate(_Delegate)
        , mWorldKey(_WorldKey)
    {
    }

    inline bool FCommandRequesterDelegate::operator==(const FObjectKey& _WorldKey) const
    {
        return mWorldKey == _WorldKey;
    }

    inline void FCommandRequesterDelegate::Broadcast(const KL::Debug::Networking::Commands::Internal::FKLDebugOnNewCommandInput& _Input) const
    {
        mDelegate.Execute(_Input);
    }

    ///////////////////////////////////////////////////////////////////////

    FKLDebugNetworkingCommandFactory mFactory;
    TArray<FCommandRequesterDelegate> RequesterDelegatesPerWorld;
    bool HasBeenInitialized = false;

    ///////////////////////////////////////////////////////////////////////

    static void AddPendingCommand(const KL::Debug::Networking::Commands::Internal::FKLDebugOnNewCommandInput& _Input)
    {
        const FObjectKey WorldKey{ &_Input.GetWorld() };
        const FCommandRequesterDelegate* Delegate = RequesterDelegatesPerWorld.FindByKey(WorldKey);
        if (Delegate)
        {
            Delegate->Broadcast(_Input);
        }
        else
        {
            UE_LOG(LogKLDebug_Networking, Error, TEXT("Could not find a valid command listener for world [%s]"), *_Input.GetWorld().GetName());
        }
    }

    /////////////////////////////////////////////////////
    /// public

    void Init()
    {
        if (HasBeenInitialized)
        {
            return;
        }

        HasBeenInitialized = true;
        RequesterDelegatesPerWorld.Reserve(10);
        KL::Debug::Networking::Commands::Internal::BindOnNewCommand(FOnKLDebugNewCommand::CreateStatic(KL::Debug::Networking::Commands::Manager::AddPendingCommand));
        mFactory.Init();
    }

    void BindOnNewCommandWorldRequest(const FObjectKey& _WorldKey, const FOnKLDebugNewCommandWorld& _Delegate)
    {
#if DO_ENSURE
        if (RequesterDelegatesPerWorld.FindByKey(_WorldKey) != nullptr)
        {
            ensureMsgf(false, TEXT("world delegate already registered should never happen"));
            return;
        }
#endif

        RequesterDelegatesPerWorld.Emplace(_WorldKey, _Delegate);
    }

    void UnbindOnNewCommandWorldRequest(const FObjectKey& _WorldKey)
    {
        const int32 Index = RequesterDelegatesPerWorld.IndexOfByKey(_WorldKey);
        ensureMsgf(Index != INDEX_NONE, TEXT("we should have found a valid delegate"));
        if (Index != INDEX_NONE)
        {
            RequesterDelegatesPerWorld.RemoveAtSwap(Index, 1, false);
        }
    }

    const IKLDebugUserNetworkingCommandInterface* TryGetCommand(const FName& _CommandID)
    {
        return mFactory.TryGetCommand(_CommandID);
    }

}    // namespace KL::Debug::Networking::Commands::Manager
