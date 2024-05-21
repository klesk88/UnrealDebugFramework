// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/KLDebugNetworkingBaseInterface.h"

// engine
#include "Templates/UnrealTypeTraits.h"

namespace KL::Debug::Networking::Getter
{
    template <typename ClassType>
    const IKLDebugNetworkingBaseInterface* TryCastToNetworkInterface(const ClassType& _ClassInstance)
    {
        if constexpr (TIsDerivedFrom<ClassType, IKLDebugNetworkingBaseInterface>::IsDerived)
        {
            return static_cast<const IKLDebugNetworkingBaseInterface*>(&_ClassInstance);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename ClassType>
    IKLDebugNetworkingBaseInterface* TryCastToNetworkInterfaceMutable(ClassType& _ClassInstance)
    {
        if constexpr (TIsDerivedFrom<ClassType, IKLDebugNetworkingBaseInterface>::IsDerived)
        {
            return static_cast<IKLDebugNetworkingBaseInterface*>(&_ClassInstance);
        }
        else
        {
            return nullptr;
        }
    }

}    // namespace KL::Debug::Networking::Getter

////////////////////////////////////////////////////////

#define KL_DEBUG_NETWORK_GETTER_INTERFACE(ItemType)                                              \
public:                                                                                          \
    const IKLDebugNetworkingBaseInterface* TryGetNetworkInterfaceInternal() const override       \
    {                                                                                            \
        return KL::Debug::Networking::Getter::TryCastToNetworkInterface<ItemType>(*this);        \
    }                                                                                            \
                                                                                                 \
    IKLDebugNetworkingBaseInterface* TryGetNetworkInterfaceMutableInternal() override            \
    {                                                                                            \
        return KL::Debug::Networking::Getter::TryCastToNetworkInterfaceMutable<ItemType>(*this); \
    }                                                                                            \
                                                                                                 \
private:    // finish KL_DEBUG_NETWORK_GETTER_INTERFACE

///////////////////////////////////////////////////////////

/*
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugNetworkingGetterInterface
{
public:
    virtual ~IKLDebugNetworkingGetterInterface();

    template <typename NetworkInterfaceClass>
    [[nodiscard]] const NetworkInterfaceClass* TryCastToNetworkInterface() const;
    template <typename NetworkInterfaceClass>
    [[nodiscard]] NetworkInterfaceClass* TryCastToNetworkInterfaceMutable();

protected:
    virtual const IKLDebugNetworkingBaseInterface* TryGetNetworkInterfaceInternal() const;
    virtual IKLDebugNetworkingBaseInterface* TryGetNetworkInterfaceMutableInternal();
};

template <typename NetworkInterfaceClass>
const NetworkInterfaceClass* IKLDebugNetworkingGetterInterface::TryCastToNetworkInterface() const
{
    if (const IKLDebugNetworkingBaseInterface* BaseNetworkInterface = TryGetNetworkInterfaceInternal())
    {
        return BaseNetworkInterface->NetworkTryCastToInterface<NetworkInterfaceClass>();
    }

    return nullptr;
}

template <typename NetworkInterfaceClass>
NetworkInterfaceClass* IKLDebugNetworkingGetterInterface::TryCastToNetworkInterfaceMutable()
{
    if (IKLDebugNetworkingBaseInterface* BaseNetworkInterface = TryGetNetworkInterfaceMutableInternal())
    {
        return BaseNetworkInterface->NetworkTryCastToInterfaceMutable<NetworkInterfaceClass>();
    }

    return nullptr;
}

inline const IKLDebugNetworkingBaseInterface* IKLDebugNetworkingGetterInterface::TryGetNetworkInterfaceInternal() const
{
    return nullptr;
}

inline IKLDebugNetworkingBaseInterface* IKLDebugNetworkingGetterInterface::TryGetNetworkInterfaceMutableInternal()
{
    return nullptr;
}