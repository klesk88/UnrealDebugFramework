#pragma once

//=================================================================================================
// Show NodeEditor Demo 
//-------------------------------------------------------------------------------------------------
// Small sample of using the node editor
// Mostly used the original example code, with a few tagged changes to make it work
// This file contains the UnrealNetImgui demo integratino code
// and 'Private/ThirdParty/imgui-node-editor/examples/' contains the original example code
//=================================================================================================

#include "CoreMinimal.h"

namespace NodeEditorDemo
{
    // Replacement for original Node Editor example Application class
    struct KLDEBUGEDITOR_API Application
    {    
        virtual ~Application(){ OnStop(); }
        virtual void OnStart() {}
        virtual void OnStop() {}
        virtual void OnFrame(float deltaTime) {}
    };

    KLDEBUGEDITOR_API void ShowDemo(bool& bVisible);
    KLDEBUGEDITOR_API void Release();
}
