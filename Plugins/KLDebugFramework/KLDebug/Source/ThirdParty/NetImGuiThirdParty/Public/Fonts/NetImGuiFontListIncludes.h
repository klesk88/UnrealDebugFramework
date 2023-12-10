#pragma once

//list of fonts. Include this file so you can get all the includes needed at once

#if NETIMGUI_FONT_ICON_GAMEKENNEY
// List of defines to easily use Icons available in 'Kenney's Game Icons'
// For list available icons, see: https://kenney.nl/assets/game-icons and https://kenney.nl/assets/game-icons-expansion
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/IconFontCppHeader/IconsKenney.h"
#endif

#if NETIMGUI_FONT_ICON_AWESOME
// List of defines to easily use Icons available in 'Font Awesome Icons' (only the 'free' subset is made available)
// For list of available icons, see: https://fontawesome.com/v6/search?m=free (Regular/Solid/Brands)
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/IconFontCppHeader/IconsFontAwesome6.h"
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/IconFontCppHeader/IconsFontAwesome6Brands.h"
#endif

#if NETIMGUI_FONT_ICON_MATERIALDESIGN
// List of defines to easily use Icons available in 'Google's Material Design Icons'
// For list of available icons, see: https://fonts.google.com/icons
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/IconFontCppHeader/IconsMaterialDesign.h"
#endif
