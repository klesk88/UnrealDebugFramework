#include "Extender/KLDebugEditorMenuExtender.h"

#include "Extender/KLDebugEditorMenuExtender_Style.h"

// KLDebugImGui module
#include "ImGui/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
// KLUnrealImGui
#include "UnrealImGui/Public/KLUnrealImGuiModule.h"
// UnrealImGui
#include "UnrealImGui/Public/ImGuiModuleProperties.h"

// engine
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Framework/SlateDelegates.h"
#include "LevelEditor.h"
#include "Modules/ModuleManager.h"
#include "Toolkits/AssetEditorToolkit.h"

#define LOCTEXT_NAMESPACE "KLDebugMenuExtender"

namespace KL::Debug::Editor::MenuExtender
{
    UE_NODISCARD bool AreRequiredModulesLoaded()
    {
        return FModuleManager::Get().IsModuleLoaded("LevelEditor") && FKLUnrealImGuiModule::TryGet() != nullptr;
    }
}  // namespace KL::Debug::Editor::MenuExtender

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void FKLDebugEditorMenuExtender::Init()
{
    FKLDebugEditorMenuExtender_Style::Init();

    if (KL::Debug::Editor::MenuExtender::AreRequiredModulesLoaded())
    {
        // Ready to register the elements
        Register();
    }
    else
    {
        // Module not yet loaded, subscribe and wait for the module to be loaded
        mOnModulesChangedHandler = FModuleManager::Get().OnModulesChanged().AddRaw(this, &FKLDebugEditorMenuExtender::OnModulesChanged);
    }
}

void FKLDebugEditorMenuExtender::Shutdown()
{
    FLevelEditorModule*    LevelEditorModule    = FModuleManager::GetModulePtr<FLevelEditorModule>("LevelEditor");
    FExtensibilityManager* ExtensibilityManager = LevelEditorModule ? LevelEditorModule->GetToolBarExtensibilityManager().Get() : nullptr;
    if (ExtensibilityManager)
    {
        ExtensibilityManager->RemoveExtender(mToolbarExtension);
        mToolbarExtension.Reset();
    }

    FKLDebugEditorMenuExtender_Style::Shutdown();
}

void FKLDebugEditorMenuExtender::OnModulesChanged(const FName _Name, const EModuleChangeReason _Reason)
{
    if (KL::Debug::Editor::MenuExtender::AreRequiredModulesLoaded())
    {
        FModuleManager::Get().OnModulesChanged().Remove(mOnModulesChangedHandler);
        mOnModulesChangedHandler.Reset();
        Register();
    }
}

// Actually registers the elements, assuming the level editor module is present.
void FKLDebugEditorMenuExtender::Register()
{
    mCommandList = MakeShareable(new FUICommandList);

    FLevelEditorModule&   LevelEditorModule  = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
    TSharedPtr<FExtender> NewToolbarExtender = MakeShareable(new FExtender);

    NewToolbarExtender->AddToolBarExtension("Play", EExtensionHook::After, mCommandList, FToolBarExtensionDelegate::CreateRaw(this, &FKLDebugEditorMenuExtender::FillToolbar, mCommandList));
    LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(NewToolbarExtender);
}

void FKLDebugEditorMenuExtender::FillToolbar(FToolBarBuilder& _Builder, TSharedPtr<FUICommandList> _CommandList)
{
    _Builder.AddComboButton(FUIAction(),
                            FOnGetContent::CreateStatic(FKLDebugEditorMenuExtender::ImGuiModuleSubMenu, _CommandList),
                            LOCTEXT("KLDebug", "KLDebug"),
                            LOCTEXT("KLDebug_Tooltip", "Tools for use with the KL Debug system"),
                            FSlateIcon(FKLDebugEditorMenuExtender_Style::GetStyleSetName(), FKLDebugEditorMenuExtender_Style::GetIconName()));
}

TSharedRef<SWidget> FKLDebugEditorMenuExtender::ImGuiModuleSubMenu(TSharedPtr<FUICommandList> _CommandList)
{
    FMenuBuilder MenuBuilder(true, _CommandList);

    MenuBuilder.BeginSection(TEXT("KLDebug.Main"), LOCTEXT("KLDebug", "KLDebug"));

    UKLDebugImGuiEngineSubsystem* KLImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    if (KLImGuiEngineSubsystem)
    {
        MenuBuilder.AddMenuEntry(LOCTEXT("KLDebugEnable", "Enable KLDebug"), FText(), FSlateIcon(),
                                 FUIAction(FExecuteAction::CreateLambda([]()
                                                                        {
                                                                            UKLDebugImGuiEngineSubsystem* KLImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
                                                                            KLImGuiEngineSubsystem->ToogleImGuiSystemState();
                                                                        }),
                                           FCanExecuteAction(), FIsActionChecked::CreateLambda([]() -> bool
                                                                                               {
                                                                                                   const UKLDebugImGuiEngineSubsystem* KLImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
                                                                                                   return KLImGuiEngineSubsystem->IsImGuiSystemEnabled();
                                                                                               })),
                                 NAME_None, EUserInterfaceActionType::ToggleButton);
    }

    MenuBuilder.EndSection();

    MenuBuilder.BeginSection(TEXT("ImGuiTools.ImGuiOptions"), LOCTEXT("ImGuiOptions", "ImGui Options"));
    MenuBuilder.AddSubMenu(LOCTEXT("ImGui_Input", "Input"), FText(), FMenuExtensionDelegate::CreateStatic(&FKLDebugEditorMenuExtender::FillImGuiSection));
    MenuBuilder.EndSection();

    return MenuBuilder.MakeWidget();
}

void FKLDebugEditorMenuExtender::FillImGuiSection(FMenuBuilder& _MenuBuilder)
{
    FKLUnrealImGuiModule* ImGuiModule = &FKLUnrealImGuiModule::Get();

    _MenuBuilder.AddMenuEntry(LOCTEXT("ImGui_InputEnabled", "Input Enabled"), FText(), FSlateIcon(),
                              FUIAction(FExecuteAction::CreateLambda([ImGuiModule]()
                                                                     {
                                                                         ImGuiModule->GetProperties().ToggleInput();
                                                                     }),
                                        FCanExecuteAction(), FIsActionChecked::CreateLambda([ImGuiModule]() -> bool
                                                                                            {
                                                                                                return ImGuiModule->GetProperties().IsInputEnabled();
                                                                                            })),
                              NAME_None, EUserInterfaceActionType::ToggleButton);

    _MenuBuilder.BeginSection(NAME_None, LOCTEXT("KeyboardMouse", "Keyboard & Mouse"));
    _MenuBuilder.AddMenuEntry(LOCTEXT("ImGui_KeyboardNav", "Keyboard Navigation"), FText(), FSlateIcon(),
                              FUIAction(FExecuteAction::CreateLambda([ImGuiModule]()
                                                                     {
                                                                         ImGuiModule->GetProperties().ToggleKeyboardNavigation();
                                                                     }),
                                        FCanExecuteAction(), FIsActionChecked::CreateLambda([ImGuiModule]() -> bool
                                                                                            {
                                                                                                return ImGuiModule->GetProperties().IsKeyboardNavigationEnabled();
                                                                                            })),
                              NAME_None, EUserInterfaceActionType::ToggleButton);

    _MenuBuilder.AddMenuEntry(LOCTEXT("ImGui_KeyboardInputShared", "Keyboard Input Shared"), FText(), FSlateIcon(),
                              FUIAction(FExecuteAction::CreateLambda([ImGuiModule]()
                                                                     {
                                                                         ImGuiModule->GetProperties().ToggleKeyboardInputSharing();
                                                                     }),
                                        FCanExecuteAction(), FIsActionChecked::CreateLambda([ImGuiModule]() -> bool
                                                                                            {
                                                                                                return ImGuiModule->GetProperties().IsKeyboardInputShared();
                                                                                            })),
                              NAME_None, EUserInterfaceActionType::ToggleButton);

    _MenuBuilder.AddMenuEntry(LOCTEXT("ImGui_MouseInputShared", "Mouse Input Shared"), FText(), FSlateIcon(),
                              FUIAction(FExecuteAction::CreateLambda([ImGuiModule]()
                                                                     {
                                                                         ImGuiModule->GetProperties().ToggleMouseInputSharing();
                                                                     }),
                                        FCanExecuteAction(), FIsActionChecked::CreateLambda([ImGuiModule]() -> bool
                                                                                            {
                                                                                                return ImGuiModule->GetProperties().IsMouseInputShared();
                                                                                            })),
                              NAME_None, EUserInterfaceActionType::ToggleButton);
    _MenuBuilder.EndSection();

    _MenuBuilder.BeginSection(NAME_None, LOCTEXT("Gamepad", "Gamepad"));

    _MenuBuilder.AddMenuEntry(LOCTEXT("ImGui_GamepadNav", "Gamepad Navigation"), FText(), FSlateIcon(),
                              FUIAction(FExecuteAction::CreateLambda([ImGuiModule]()
                                                                     {
                                                                         ImGuiModule->GetProperties().ToggleGamepadNavigation();
                                                                     }),
                                        FCanExecuteAction(), FIsActionChecked::CreateLambda([ImGuiModule]() -> bool
                                                                                            {
                                                                                                return ImGuiModule->GetProperties().IsGamepadNavigationEnabled();
                                                                                            })),
                              NAME_None, EUserInterfaceActionType::ToggleButton);

    _MenuBuilder.AddMenuEntry(LOCTEXT("ImGui_KeyboardInputShared", "Gamepad Input Shared"), FText(), FSlateIcon(),
                              FUIAction(FExecuteAction::CreateLambda([ImGuiModule]()
                                                                     {
                                                                         ImGuiModule->GetProperties().ToggleGamepadInputSharing();
                                                                     }),
                                        FCanExecuteAction(), FIsActionChecked::CreateLambda([ImGuiModule]() -> bool
                                                                                            {
                                                                                                return ImGuiModule->GetProperties().IsGamepadInputShared();
                                                                                            })),
                              NAME_None, EUserInterfaceActionType::ToggleButton);

    _MenuBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE
