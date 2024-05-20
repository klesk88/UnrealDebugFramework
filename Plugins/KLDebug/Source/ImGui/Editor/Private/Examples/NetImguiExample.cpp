#include "Examples/NetImguiExample.h"

// modules
#include "ThirdParty/NetImGuiThirdParty/Public/Fonts/NetImGuiFontListIncludes.h"
#include "UnrealImGui/Public/Font/KLUnrealImGuiFontDefines.h"
#include "UnrealImGui/Public/Font/KLUnrealImGuiScopedFont.h"
#include "User/ThirdParty/ImGui/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "GenericPlatform/GenericPlatform.h"

#if NETIMGUI_FONT_ICON_AWESOME
#define CLIENTSTRING_NAME ICON_FA_CIRCLE_INFO " Name"
#define CLIENTSTRING_SHOW ICON_FA_EYE
#define CLIENTSTRING_POS  ICON_FA_LOCATION_DOT " Position"

#elif NETIMGUI_FONT_ICON_MATERIALDESIGN
#define CLIENTSTRING_NAME ICON_MD_INFO " Name"
#define CLIENTSTRING_SHOW ICON_MD_VISIBILITY
#define CLIENTSTRING_POS  ICON_MD_PLACE " Position"

#else
#define CLIENTSTRING_NAME "Name"
#define CLIENTSTRING_SHOW "Position"
#define CLIENTSTRING_POS  "Visible"
#endif

namespace NetImGui::Demo
{
    static const ImVec4 kColorHighlight = ImVec4(0.1f, 0.85f, 0.1f, 1.0f);

    void DrawDemo(bool* _bOpen)
    {
        if (!_bOpen || !(*_bOpen))
        {
            return;
        }

        if (ImGui::Begin("DemoActor: PerFrame"))
        {
            ImGui::TextWrapped("This Windows is drawn once per frame (when NetImguiServer request it), using a NetImgui callback.");
            ImGui::NewLine();

            //-------------------------------------------------------------------------------------
            // Demonstrations of using icons and japanese font
            // Note:	To create a utf8 string that can represent unicode character,
            //			prefix it with 'u8'
            //-------------------------------------------------------------------------------------
            if (ImGui::CollapsingHeader("Icons & Font", ImGuiTreeNodeFlags_DefaultOpen))
            {
                uint32 iconAnimFrame = (GFrameNumber / 60);

#if NETIMGUI_FONT_JAPANESE
                ImGui::TextColored(kColorHighlight, "Japanese font");
                {
                    //--- Showcase using a utf8 string mixing japanese and latin content ---
                    FKLUnrealImGuiScopedFont iconFont(KL::UnrealImGui::Fonts::eFont::kJapanese32);
                    ImGui::TextWrapped(u8"日本語とカタカナとひらがなとlatinを使用することができます。やった！");
                }
#endif

#if NETIMGUI_FONT_ICON_GAMEKENNEY
                //--- Showcase using a FString to mix icon and text together ---
                ImGui::NewLine();
                FString titleKenney = FString::Format(TEXT("{0} Game Kenney Icons"), { UTF8_TO_TCHAR(ICON_KI_INFO_CIRCLE) });
                ImGui::TextColored(kColorHighlight, "%s", TCHAR_TO_ANSI(*titleKenney));

                //--- Showcase using multiple strings that includes normal text and icons, merged together in 1 utf8 string constant ---
                ImGui::TextUnformatted("I " ICON_KI_HEART " icons in my text.");
                {
                    FKLUnrealImGuiScopedFont iconFont(KL::UnrealImGui::Fonts::eFont::kIcons64);
                    const char* zAnimation[] = { ICON_KI_DPAD, ICON_KI_DPAD_TOP, ICON_KI_DPAD_RIGHT, ICON_KI_DPAD_BOTTOM, ICON_KI_DPAD_LEFT };
                    ImGui::Text(ICON_KI_GAMEPAD " %s " ICON_KI_BUTTON_SELECT " " ICON_KI_BUTTON_START " " ICON_KI_BUTTON_A " " ICON_KI_BUTTON_B, zAnimation[iconAnimFrame % UE_ARRAY_COUNT(zAnimation)]);
                }
#endif

#if NETIMGUI_FONT_ICON_AWESOME
                //--- Showcase using a FString to mix icon and text together ---
                FString titleAwesome = FString::Format(TEXT("{0} Font Awesome Icons"), { UTF8_TO_TCHAR(ICON_FA_CIRCLE_INFO) });
                ImGui::TextColored(kColorHighlight, "%s", TCHAR_TO_ANSI(*titleAwesome));

                //--- Showcase using a utf8 string with icons inserted in it as a regular printf string constant ---
                ImGui::Text("I %s icons in my text.", ICON_FA_HEART);
                {
                    FKLUnrealImGuiScopedFont iconFont(KL::UnrealImGui::Fonts::eFont::kIcons64);
                    const char* zAnimation[] = { ICON_FA_BATTERY_EMPTY, ICON_FA_BATTERY_QUARTER, ICON_FA_BATTERY_HALF, ICON_FA_BATTERY_THREE_QUARTERS, ICON_FA_BATTERY_FULL };
                    ImGui::Text(ICON_FA_FACE_SMILE " " ICON_FA_MINIMIZE " " ICON_FA_GEAR " " ICON_FA_ARROW_ROTATE_LEFT " " ICON_FA_ARROW_ROTATE_RIGHT " %s", zAnimation[iconAnimFrame % UE_ARRAY_COUNT(zAnimation)]);    // Font awesome standard icons
                    ImGui::Text(ICON_FA_CANADIAN_MAPLE_LEAF " " ICON_FA_JIRA " " ICON_FA_ATLASSIAN " " ICON_FA_ANDROID);                                                                                                 // Font Awesome free icons examples
                }
#endif

#if NETIMGUI_FONT_ICON_MATERIALDESIGN
                //--- Showcase using a FString to mix icon and text together ---
                FString titleMaterial = FString::Format(TEXT("{0} Material Design Icons"), { UTF8_TO_TCHAR(ICON_MD_INFO) });
                ImGui::TextColored(kColorHighlight, TCHAR_TO_UTF8(*titleMaterial));
                //--- Showcase using a utf8 string with icons inserted in it as a regular printf string constant ---
                ImGui::TextUnformatted("I " ICON_MD_FAVORITE " icons in my text.");
                {
                    FKLUnrealImGuiScopedFont iconFont(KL::UnrealImGui::Fonts::eFont::kIcons64);
                    const char* zAnimation[] = { ICON_MD_BRIGHTNESS_1, ICON_MD_BRIGHTNESS_2, ICON_MD_BRIGHTNESS_3, ICON_MD_BRIGHTNESS_4, ICON_MD_BRIGHTNESS_5, ICON_MD_BRIGHTNESS_6, ICON_MD_BRIGHTNESS_7 };
                    ImGui::Text(ICON_MD_SENTIMENT_SATISFIED " " ICON_MD_OPEN_WITH " " ICON_MD_SETTINGS " " ICON_MD_KEYBOARD_ARROW_LEFT " " ICON_MD_KEYBOARD_ARROW_RIGHT " %s", zAnimation[iconAnimFrame % UE_ARRAY_COUNT(zAnimation)]);
                }
#endif
            }

            //-------------------------------------------------------------------------------------
            // Display a list of actor in the Scene
            //-------------------------------------------------------------------------------------
            ImGui::NewLine();
            if (ImGui::CollapsingHeader("Actors", ImGuiTreeNodeFlags_DefaultOpen))
            {
                static uint32 sCount = 0;
                if (ImGui::BeginTable("Actors List", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable))
                {
                    // Table header, with a specific font assigned
                    {
                        FKLUnrealImGuiScopedFont headerFont(KL::UnrealImGui::Fonts::eFont::kCousineFixed20);
                        ImVec4 tableBgColor = ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg);
                        tableBgColor.w = 1.f;
                        ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, tableBgColor);
                        ImGui::TableSetupColumn(CLIENTSTRING_NAME);
                        ImGui::TableSetupColumn(CLIENTSTRING_POS);
                        ImGui::TableSetupColumn(CLIENTSTRING_SHOW, ImGuiTableColumnFlags_WidthFixed, 40.0f);
                        if (ImGui::TableGetColumnFlags(2) & ImGuiTableColumnFlags_IsHovered)
                        {
                            ImGui::SetTooltip("Toggle visibility (Editor only)");
                        }
                        ImGui::TableHeadersRow();
                        ImGui::PopStyleColor();
                    }

                    // Table content, with list of actors in the world
                    sCount = 0;
                    for (TActorIterator<AActor> It(GWorld); It; ++It)
                    {
                        AActor* pActor = *It;
                        if (pActor != nullptr && pActor->GetOwner() == nullptr)
                        {
                            int uniqueId = GetTypeHash(pActor->GetFName());
                            ImGui::PushID(uniqueId);

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::TextUnformatted(TCHAR_TO_UTF8(*pActor->GetName()));

                            ImGui::TableNextColumn();
                            FVector pos = pActor->GetTransform().GetLocation();
                            ImGui::Text("(%.02f, %.02f, %.02f)", pos.X, pos.Y, pos.Z);

                            ImGui::TableNextColumn();
#if WITH_EDITOR
                            bool bVisible = !pActor->IsHiddenEd();
                            if (ImGui::Checkbox("", &bVisible))
                            {
                                pActor->SetIsTemporarilyHiddenInEditor(!bVisible);
                            }
#else
                            bool bVisible = !pActor->IsHidden();
                            ImGui::Checkbox("", &bVisible);
#endif
                            ImGui::PopID();
                            sCount++;
                        }
                    }
                    ImGui::EndTable();
                }
            }
        }
        ImGui::End();
    }
}    // namespace NetImGui::Demo
