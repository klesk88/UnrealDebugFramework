// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Config/KLDebugImGuiConfig.h"

#include "Window/KLDebugImGuiWindow.h"

UKLDebugImGuiConfig::UKLDebugImGuiConfig()
    : UDeveloperSettings()
    , ImGuiWindowManager(FKLDebugImGuiWindow::StaticStruct())
{
    CategoryName = TEXT("Plugins");
    SectionName = TEXT("KLImGui");
}
