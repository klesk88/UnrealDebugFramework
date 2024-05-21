// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"

// engine
#include "Templates/UniquePtr.h"

class FKLDebugContextGetterInput;

/*
 * base interface class use by other interface to implement a way to generate a context runtime
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugContextUserInterface
{
public:
    virtual ~IKLDebugContextUserInterface();

    [[nodiscard]] virtual TUniquePtr<IKLDebugContextInterface> GetContext(const FKLDebugContextGetterInput& _Input) const;
};

inline TUniquePtr<IKLDebugContextInterface> IKLDebugContextUserInterface::GetContext(const FKLDebugContextGetterInput& _Input) const
{
    return nullptr;
}