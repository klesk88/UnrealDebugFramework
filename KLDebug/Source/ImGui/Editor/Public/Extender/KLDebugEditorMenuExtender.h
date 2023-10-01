// Distributed under the MIT License (MIT) (see accompanying LICENSE file)
#pragma once

// engine
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "Framework/Commands/UICommandList.h"
#include "Templates/SharedPointer.h"
#include "Widgets/SWidget.h"

class FMenuBuilder;
class FToolBarBuilder;

// TODO: should we move all this in a separate editor module without any dependency from mine (so it can be included by this and the imgui one) and
// have a callback system to populate the entries?

/*
 * based on code from github project of nakdeyes/UnrealImGuiTools
 * this allow us to create a small button in editor after the play button to setup settings of the debug system
 */
class KLDEBUGIMGUIEDITOR_API FKLDebugEditorMenuExtender
{
public:
    void Init();
    void Shutdown();

private:
    void OnModulesChanged(const FName _Name, const EModuleChangeReason _Reason);
    void Register();

    void FillToolbar(FToolBarBuilder& _Builder, TSharedPtr<FUICommandList> _CommandList);

    static TSharedRef<SWidget> ImGuiModuleSubMenu(TSharedPtr<FUICommandList> _CommandList);
    static void                FillImGuiSection(FMenuBuilder& MenuBuilder);

private:
    TSharedPtr<FUICommandList> mCommandList;
    FDelegateHandle            mOnModulesChangedHandler;
    TSharedPtr<FExtender>      mToolbarExtension;
};