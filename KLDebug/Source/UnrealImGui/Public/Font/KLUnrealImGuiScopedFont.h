#pragma once

#include "Font/KLUnrealImGuiFontDefines.h"

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class KLUNREALIMGUI_API FKLUnrealImGuiScopedFont final : public FNoncopyable
{
public:
    FKLUnrealImGuiScopedFont(KL::UnrealImGui::Fonts::eFont _Font);
    ~FKLUnrealImGuiScopedFont();
};