#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/Class.h"

#include "KLDebugImGuiWindow.generated.h"

class UWorld;

USTRUCT()
struct KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiWindow
{
    GENERATED_BODY()

public:
    virtual ~FKLDebugImGuiWindow() = default;

    virtual void Init();
    virtual void Shutdown();
    virtual void Update(const UWorld& _World);

protected:
    virtual void DrawImGuiTopBar(const UWorld& _World) const;
    virtual void DrawImGuiBar() const;
    virtual void DrawImGuiBottomBar(const UWorld& _World) const;

    void DrawImGuiEngine() const;

private:
    bool mDrawBottomBar = true;

#if IMGUI_UNREAL_COMMAND_ENABLED
private:
    void DrawCommandsMenu() const;
    void DrawCommands() const;
#endif
};
