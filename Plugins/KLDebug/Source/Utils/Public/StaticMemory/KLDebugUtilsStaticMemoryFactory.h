// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

// engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

template <typename DataInterface>
class TKLDebugUtilsStaticMemoryFactory : public FNoncopyable
{
protected:
    template <typename StaticMemoryManagerType, typename StaticMemoryEntryType>
    void InitInternal();

    UE_NODISCARD const DataInterface* TryGetData(const FName& _DataID) const;
    UE_NODISCARD DataInterface* TryGetDataMutable(const FName& _DataID);

    UE_NODISCARD const DataInterface& GetData(const uint32 _Index) const;
    UE_NODISCARD DataInterface& GetDataMutable(const uint32 _Index);

protected:
    TMap<FName, uint32> mIDToData;
    TArray<uint8> mDataPool;
};

template <typename DataInterface>
typename const DataInterface* TKLDebugUtilsStaticMemoryFactory<DataInterface>::TryGetData(const FName& _DataID) const
{
    if (const uint32* Offset = mIDToData.Find(_DataID))
    {
        return &GetData(*Offset);
    }

    return nullptr;
}

template <typename DataInterface>
typename DataInterface* TKLDebugUtilsStaticMemoryFactory<DataInterface>::TryGetDataMutable(const FName& _DataID)
{
    if (const uint32* Offset = mIDToData.Find(_DataID))
    {
        return &GetDataMutable(*Offset);
    }

    return nullptr;
}

template <typename DataInterface>
typename const DataInterface& TKLDebugUtilsStaticMemoryFactory<DataInterface>::GetData(const uint32 _Index) const
{
    checkf(mDataPool.IsValidIndex(_Index), TEXT("index must be valid"));
    return reinterpret_cast<const DataInterface&>(mDataPool[_Index]);
}

template <typename DataInterface>
typename DataInterface& TKLDebugUtilsStaticMemoryFactory<DataInterface>::GetDataMutable(const uint32 _Index)
{
    checkf(mDataPool.IsValidIndex(_Index), TEXT("index must be valid"));
    return reinterpret_cast<DataInterface&>(mDataPool[_Index]);
}

template <typename DataInterface>
template <typename StaticMemoryManagerType, typename StaticMemoryEntryType>
void TKLDebugUtilsStaticMemoryFactory<DataInterface>::InitInternal()
{
    StaticMemoryManagerType& Manager = StaticMemoryManagerType::Get();
    mIDToData.Reserve(static_cast<int32>(Manager.GetEntryCount()));
    mDataPool.Init(0, Manager.GetTotalSizeRequired());
    SIZE_T Offset = 0;

    const StaticMemoryEntryType* StaticMemoryEntry = Manager.GetStartEntry();
    while (StaticMemoryEntry)
    {
        const DataInterface& Interface = StaticMemoryEntry->AllocateInPlace(static_cast<void*>(&mDataPool[static_cast<int32>(Offset)]));
        const FName& Key = Interface.GetID();
        checkf(Offset < TNumericLimits<uint32>::Max(), TEXT("too many data"));

#if DO_ENSURE
        if (mIDToData.Find(Key) != nullptr)
        {
            ensureAlwaysMsgf(mIDToData.Find(Key) == nullptr, TEXT("Multiple data with the same type. should never happen"));
            continue;
        }
#endif

        mIDToData.Emplace(Interface.GetID(), static_cast<uint32>(Offset));
        Offset += StaticMemoryEntry->GetSize();
        StaticMemoryEntry = StaticMemoryEntry->GetNextEntry();
    }
}
