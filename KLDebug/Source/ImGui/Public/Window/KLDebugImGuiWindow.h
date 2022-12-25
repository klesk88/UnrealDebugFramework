#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

#include "KLDebugImGuiWindow.generated.h"

class UWorld;

USTRUCT()
struct KLDEBUGIMGUI_API FKLDebugImGuiWindow
{
    GENERATED_BODY()

public:
    virtual void Update(const UWorld& _World);

protected:
    virtual void DrawImGuiTopBar(const UWorld& _World) const;
    virtual void DrawImGuiBar() const;
    virtual void DrawImGuiBottomBar(const UWorld& _World) const;

    void DrawImGuiEngine() const; 

private:
    bool mDrawBottomBar = true;
};
