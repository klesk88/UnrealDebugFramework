// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Font/KLUnrealImGuiFontManager.h"

// modules
#include "ThirdParty/NetImGuiThirdParty/Public/Library/NetImgui_Api.h"
#include "ThirdParty/UnrealImGui/Public/ImGuiContextManager.h"
#include "User/ThirdParty/ImPlot/Public/Library/implot.h"

// engine
#include "Math/UnrealMathUtility.h"

//=================================================================================================
// Binary Font converted to c data array
// (using Dear Imgui 'binary_to_compressed_c.cpp')
//=================================================================================================
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/Cousine_Regular.cpp"
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/Droid_Sans.cpp"
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/Karla_Regular.cpp"
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/Proggy_Tiny.cpp"
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/Roboto_Medium.cpp"

#if NETIMGUI_FREETYPE_ENABLED
#include "User/ThirdParty/ImGui/Public/Library/misc/freetype/imgui_freetype.h"
#endif

#if NETIMGUI_FONT_ICON_GAMEKENNEY
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/FontKenney/KenneyIcon.cpp"
// List of defines to easily use Icons available in 'Kenney's Game Icons'
// For list available icons, see: https://kenney.nl/assets/game-icons and https://kenney.nl/assets/game-icons-expansion
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/IconFontCppHeader/IconsKenney.h"
#endif

#if NETIMGUI_FONT_ICON_AWESOME
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/FontAwesome6/fa-brands-400.cpp"
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/FontAwesome6/fa-regular-400.cpp"
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/FontAwesome6/fa-solid-900.cpp"
// List of defines to easily use Icons available in 'Font Awesome Icons' (only the 'free' subset is made available)
// For list of available icons, see: https://fontawesome.com/v6/search?m=free (Regular/Solid/Brands)
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/IconFontCppHeader/IconsFontAwesome6.h"
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/IconFontCppHeader/IconsFontAwesome6Brands.h"
#endif

#if NETIMGUI_FONT_ICON_MATERIALDESIGN
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/FontMaterialDesign/MaterialIcons_Regular.cpp"
// List of defines to easily use Icons available in 'Google's Material Design Icons'
// For list of available icons, see: https://fonts.google.com/icons
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/IconFontCppHeader/IconsMaterialDesign.h"
#endif

#if NETIMGUI_FONT_JAPANESE
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/FontIPAexMincho/IPAexMincho.cpp"
#endif

void FKLUnrealImGuiFontManager::PushFont(const KL::UnrealImGui::Fonts::eFont _Font)
{
    check(_Font < KL::UnrealImGui::Fonts::eFont::_Count);
    ImFont* pFont = _Font < KL::UnrealImGui::Fonts::eFont::_Count ? ImGui::GetIO().Fonts->Fonts[static_cast<int>(_Font)] : nullptr;
    ImGui::PushFont(pFont ? pFont : ImGui::GetFont());
}

void FKLUnrealImGuiFontManager::PopFont()
{
    ImGui::PopFont();
}

void FKLUnrealImGuiFontManager::SetDefaultFont(const KL::UnrealImGui::Fonts::eFont _Font)
{
    check(_Font < KL::UnrealImGui::Fonts::eFont::_Count);
    if (ImGui::GetIO().Fonts->Fonts[static_cast<int32>(_Font)])
    {
        ImFont* pFont = _Font < KL::UnrealImGui::Fonts::eFont::_Count ? ImGui::GetIO().Fonts->Fonts[static_cast<int>(_Font)] : nullptr;
        ImGui::GetIO().FontDefault = pFont ? pFont : ImGui::GetIO().FontDefault;
    }
}

void FKLUnrealImGuiFontManager::Init(FImGuiContextManager& _ContextManager)
{
    _ContextManager.mOnBuildFontAtlasDelegate.BindRaw(this, &FKLUnrealImGuiFontManager::BuildFontAtlas);
}

void FKLUnrealImGuiFontManager::Shutdown(FImGuiContextManager& _ContextManager)
{
    _ContextManager.mOnBuildFontAtlasDelegate.Unbind();
}

void FKLUnrealImGuiFontManager::BuildFontAtlas(const float _DPIScale, ImFontAtlas& _OutFontAtlas) const
{
    ImFontConfig Config;
    Config.SizePixels = FMath::RoundFromZero(13.f * _DPIScale);
#if NETIMGUI_FREETYPE_ENABLED
    Config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LightHinting;    // Without this, kanji character looks wrong in smaller font size
#endif

    _OutFontAtlas.AddFontDefault(&Config);

    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("Cousine Fixed"), 16.f, Cousine_Regular_compressed_data, Cousine_Regular_compressed_size, true, true);
    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("Cousine Fixed"), 20.f, Cousine_Regular_compressed_data, Cousine_Regular_compressed_size, true, true);
    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("Cousine Fixed"), 24.f, Cousine_Regular_compressed_data, Cousine_Regular_compressed_size, true, true);
    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("Droid Sans"), 20.f, Droid_Sans_compressed_data, Droid_Sans_compressed_size, true);
    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("Karla Regular"), 16.f, Karla_Regular_compressed_data, Karla_Regular_compressed_size, true);
    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("Proggy Tiny"), 10.f, Proggy_Tiny_compressed_data, Proggy_Tiny_compressed_size, false);
    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("Roboto Medium"), 16.f, Roboto_Medium_compressed_data, Roboto_Medium_compressed_size, true);
    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("Icons"), 32.f, Cousine_Regular_compressed_data, Cousine_Regular_compressed_size, true);
    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("Icons"), 64.f, Cousine_Regular_compressed_data, Cousine_Regular_compressed_size, true);
#if NETIMGUI_FONT_JAPANESE
    ImGuiIO& io = ImGui::GetIO();
    AddFontGroup(_OutFontAtlas, Config, _DPIScale, TEXT("日本語"), 32.f, IPAexMincho_compressed_data, IPAexMincho_compressed_size, true, false, io.Fonts->GetGlyphRangesJapanese());
#endif
}

void FKLUnrealImGuiFontManager::AddFontGroup(ImFontAtlas& _OutFontAtlas, ImFontConfig& _FontConfig, const float _DPIScale, FString _Name, const float _PxSize, const uint32_t* _FontData, const uint32_t _FontDataSize, const bool _ExtraIconGlyphs, const bool AppendJapanese /* = false*/, const ImWchar* _GlyphRange /* = nullptr*/) const
{
    _FontConfig.MergeMode = false;

    _Name += FString::Printf(TEXT(" (%ipx)"), static_cast<int>(_PxSize));
    FPlatformString::Strcpy(_FontConfig.Name, sizeof(_FontConfig.Name), TCHAR_TO_UTF8(_Name.GetCharArray().GetData()));
    _OutFontAtlas.AddFontFromMemoryCompressedTTF(_FontData, _FontDataSize, _PxSize, &_FontConfig, _GlyphRange);

    _FontConfig.MergeMode = true;
#if NETIMGUI_FONT_JAPANESE
    if (AppendJapanese)
    {
        _FontConfig.RasterizerMultiply = 1.5f;    // Boost kanji color intensity slightly, making them more readable
        _OutFontAtlas.AddFontFromMemoryCompressedTTF(IPAexMincho_compressed_data, IPAexMincho_compressed_size, _PxSize, &_FontConfig, pFontAtlas->GetGlyphRangesJapanese());
        _FontConfig.RasterizerMultiply = 1.f;
    }
#endif

    if (_ExtraIconGlyphs)
    {
        _FontConfig.GlyphOffset.y = _PxSize * 0.2f;    // Try aligning the icons a little more with the text, preventing icon to overlap previous text line

#if NETIMGUI_FONT_ICON_GAMEKENNEY
        static const ImWchar iconKenney_ranges[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };
        _OutFontAtlas.AddFontFromMemoryCompressedTTF(KenneyIcon_compressed_data, KenneyIcon_compressed_size, _PxSize, &_FontConfig, iconKenney_ranges);
#endif
#if NETIMGUI_FONT_ICON_AWESOME
        static const ImWchar iconFontAwesome_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        static const ImWchar iconFontAwesomeBrands_ranges[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };
        _OutFontAtlas.AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, _PxSize, &_FontConfig, iconFontAwesome_ranges);
        _OutFontAtlas.AddFontFromMemoryCompressedTTF(fa_regular_400_compressed_data, fa_regular_400_compressed_size, _PxSize, &_FontConfig, iconFontAwesome_ranges);
        _OutFontAtlas.AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, _PxSize, &_FontConfig, iconFontAwesomeBrands_ranges);
#endif
#if NETIMGUI_FONT_ICON_MATERIALDESIGN
        static const ImWchar iconMaterialDesign_ranges[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };
        _OutFontAtlas.AddFontFromMemoryCompressedTTF(MaterialIcons_Regular_compressed_data, MaterialIcons_Regular_compressed_size, _PxSize, &_FontConfig, iconMaterialDesign_ranges);
#endif
    }
}
