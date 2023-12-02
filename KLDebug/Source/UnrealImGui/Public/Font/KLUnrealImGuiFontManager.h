// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Font/KLUnrealImGuiFontDefines.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"

class FImGuiContextManager;

struct ImFontAtlas;

class KLUNREALIMGUI_API FKLUnrealImGuiFontManager final : public FNoncopyable
{
public:
    static void PushFont(const KL::UnrealImGui::Fonts::eFont font);
    static void PopFont();
    static void SetDefaultFont(const KL::UnrealImGui::Fonts::eFont _Font);

    void Init(FImGuiContextManager& _ContextManager);
    void Shutdown(FImGuiContextManager& _ContextManager);

private:
    void BuildFontAtlas(const float _DPIScale, ImFontAtlas& _OutFontAtlas) const;
    void AddFontGroup(ImFontAtlas& _OutFontAtlas, ImFontConfig& _FontConfig, const float _DPIScale, FString _Name, const float _PxSize, const uint32_t* _FontData, const uint32_t _FontDataSize, const bool _ExtraIconGlyphs, const bool AppendJapanese = false, const ImWchar* _GlyphRange = nullptr) const;
};
