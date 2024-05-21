// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/UnrealString.h"
#include "UObject/Class.h"

#include "KLDebugImGuiConfig_Canvas.generated.h"

USTRUCT()
struct KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiConfig_Canvas
{
    GENERATED_BODY()

public:
    [[nodiscard]] float GetBorderOffsetX() const;
    [[nodiscard]] float GetBorderOffsetY() const;

private:
    UPROPERTY(EditDefaultsOnly, config)
    float CanvasBorderOffssetX = 10.f;

    UPROPERTY(EditDefaultsOnly, config)
    float CanvasBorderOffssetY = 30.f;
};

inline float FKLDebugImGuiConfig_Canvas::GetBorderOffsetX() const
{
    return CanvasBorderOffssetX;
}

inline float FKLDebugImGuiConfig_Canvas::GetBorderOffsetY() const
{
    return CanvasBorderOffssetY;
}
