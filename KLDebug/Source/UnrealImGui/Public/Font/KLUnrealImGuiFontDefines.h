#pragma once

// engine
#include "CoreMinimal.h"

namespace KL::UnrealImGui::Fonts
{
    enum class eFont
    {
        kProggyClean,     // Built-in Dear ImGui Font
        kCousineFixed16,  // Fixed size font + Japanese Mincho font + 'Kenney Game Icons' + 'Font Awesome Icons' or 'Material Design Icons'
        kCousineFixed20,  //  "
        kCousineFixed24,  //  "
        kDroidSans,       // TrueType font + 'Kenney Game Icons' + 'Font Awesome Icons' or 'Material Design Icons'
        kKarlaRegular,    // TrueType font + 'Kenney Game Icons' + 'Font Awesome Icons' or 'Material Design Icons'
        kProggyTiny,      // Tiny debug font
        kRobotoMedium16,  // TrueType font + 'Kenney Game Icons' + 'Font Awesome Icons' or 'Material Design Icons'
        kIcons32,         // Big Icons font with 'Kenney Game Icons' + 'Font Awesome Icons' or 'Material Design Icons'
        kIcons64,         //  "
#if NETIMGUI_FONT_JAPANESE
        kJapanese32,  // Japanese Mincho font + 'Kenney Game Icons' + 'Font Awesome Icons' or 'Material Design Icons'
#endif

        //... Your own font can be added here and loaded in 'FNetImguiModule::StartupModule()' in same order
        //... Feel free to also add/remove font size for your convenience
        _Count,
    };
}