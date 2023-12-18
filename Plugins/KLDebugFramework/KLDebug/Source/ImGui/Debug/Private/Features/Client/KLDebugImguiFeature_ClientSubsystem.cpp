// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/Client/KLDebugImguiFeature_ClientSubsystem.h"

// modules
#include "ImGui/Client/Public/Subsystem/Engine/KLDebugImGuiClientSubsystem_Engine.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"
#include "User/Public/Feature/Interface/Unique/Input/KLDebugImGuiFeatureImGuiInput_Unique.h"
#include "User/Public/Feature/Interface/Unique/Input/KLDebugImGuiFeatureRenderInput_Unique.h"
#include "User/Public/Helpers/KLDebugImGuiHelpers.h"

// engine
#include "Engine/World.h"

KL_DEBUG_CREATE_WINDOW(FKLDebugImguiFeature_ClientSubsystem)

bool FKLDebugImguiFeature_ClientSubsystem::DoesSupportObject(const UObject& _Object) const
{
    return UKLDebugImGuiClientSubsystem_Engine::TryGet() != nullptr;
}

const FString& FKLDebugImguiFeature_ClientSubsystem::GetWindowName() const
{
    static const FString Name(TEXT("ClientSubsystem"));
    return Name;
}

const FName& FKLDebugImguiFeature_ClientSubsystem::GetImGuiPath() const
{
    static const FName Path(TEXT("KL.Debug.Client"));
    return Path;
}

void FKLDebugImguiFeature_ClientSubsystem::DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input)
{
    UKLDebugImGuiClientSubsystem_Engine* ClientEngine = UKLDebugImGuiClientSubsystem_Engine::TryGetMutable();
    if (ClientEngine)
    {
        ClientEngine->DrawImgui(_Input.GetWorld());
    }
}
