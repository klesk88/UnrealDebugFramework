// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Font/KLUnrealImGuiScopedFont.h"

#include "Font/KLUnrealImGuiFontManager.h"

FKLUnrealImGuiScopedFont::FKLUnrealImGuiScopedFont(KL::UnrealImGui::Fonts::eFont _Font)
{
    FKLUnrealImGuiFontManager::PushFont(_Font);
}

FKLUnrealImGuiScopedFont::~FKLUnrealImGuiScopedFont()
{
    FKLUnrealImGuiFontManager::PopFont();
}
