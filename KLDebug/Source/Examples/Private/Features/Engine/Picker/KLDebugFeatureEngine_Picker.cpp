#include "Features/Engine/Picker/KLDebugFeatureEngine_Picker.h"

// imgui module
#include "ImGui/Public/Helpers/KLDebugImGuiHelpers.h"
#include "ImGui/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

//engine
#include "Engine/World.h"
#include "GameFramework/Character.h"

KL_DEBUG_CREATE_WINDOW(FKLDebugFeatureEngine_Picker)

FKLDebugFeatureEngine_Picker::FKLDebugFeatureEngine_Picker()
{
    mPicker.SetActorType(*ACharacter::StaticClass());
}

const FString& FKLDebugFeatureEngine_Picker::GetWindowName() const
{
    static const FString Name(TEXT("ActorPicker"));
    return Name;
}

const FName& FKLDebugFeatureEngine_Picker::GetImGuiPath() const
{
    static const FName Path(TEXT("Engine.Utils"));
    return Path;
}

void FKLDebugFeatureEngine_Picker::DrawImGuiChild(const FKLDebugImGuiFeatureInterfaceImGuiInput_Subsystem& _Input)
{
    if (ImGui::Button("PerformPick"))
    {
        HandlePickerButton(_Input.GetWorld());
    }

    ImguiDrawPickedCharacterInfo();
}

void FKLDebugFeatureEngine_Picker::HandlePickerButton(const UWorld& _World)
{
    UObject* SelectedUObject = mPicker.GetPickedObject(_World);
    mPickedActor = Cast<AActor>(SelectedUObject);
    if (!mPickedActor.IsValid())
    {
        return;
    }

    if (UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = UKLDebugImGuiWorldSubsystem::TryGetMutable(_World))
    {
        ImGuiWorldSubsystem->OnObjectSelected(*mPickedActor.Get());
    }
}

void FKLDebugFeatureEngine_Picker::ImguiDrawPickedCharacterInfo() const
{
    if (!mPickedActor.IsValid())
    {
        ImGui::Text("No valid actor picked");
        return;
    }

    KL::Debug::ImGuiHelpers::DrawColoredText(FColor::Green, mPickedActor->GetName());
}
