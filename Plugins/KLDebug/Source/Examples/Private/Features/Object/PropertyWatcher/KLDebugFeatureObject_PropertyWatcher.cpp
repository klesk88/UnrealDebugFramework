// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/Object/PropertyWatcher/KLDebugFeatureObject_PropertyWatcher.h"

// modules
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"
#include "User/Framework/Public/ThirdParty/ImGui/Helpers/KLDebugImGuiHelpers.h"

// engine
#include "Engine/World.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UnrealType.h"

KL_DEBUG_CREATE_WINDOW(FKLDebugFeatureObject_PropertyWatcher)

const FString& FKLDebugFeatureObject_PropertyWatcher::GetWindowName() const
{
    static const FString Name(TEXT("PropertyWatcher"));
    return Name;
}

void FKLDebugFeatureObject_PropertyWatcher::GetFilterPath(TArray<FName>& _OutFilters) const
{
}

const FName& FKLDebugFeatureObject_PropertyWatcher::GetImGuiPath() const
{
    static const FName Path(TEXT("Utils.PropertyWatcher"));
    return Path;
}

void FKLDebugFeatureObject_PropertyWatcher::DrawImGuiChild(FKLDebugFeatureImGuiInput_Selectable& _Input)
{
    // Based on PropertyWatcher project
    // https://github.com/guitarfreak/PropertyWatcher/blob/main/PropertyWatcher.cpp#L2179

    if (!ImGui::BeginTable("Actors List", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable))
    {
        return;
    }

    // Table header, with a specific font assigned
    ImVec4 TableBgColor = ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg);
    TableBgColor.w = 1.f;
    ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, TableBgColor);
    ImGui::TableSetupColumn("PropertyName");
    ImGui::TableSetupColumn("PropertyValue");
    ImGui::TableHeadersRow();
    ImGui::PopStyleColor();

    FString VariableName;
    TArray<char> StringBuffer;
    StringBuffer.Reserve(300);
    for (TFieldIterator<FProperty> It(_Input.GetObject().GetClass()); It; ++It)
    {
        FProperty* Property = *It;
        if (!Property)
        {
            continue;
        }

        VariableName = Property->GetName();
        const int32 UniqueId = GetTypeHash(Property->GetFName());
        ImGui::PushID(UniqueId);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TextUnformatted(TCHAR_TO_UTF8(*Property->GetName()));
        ImGui::TableNextColumn();
        ImguiDrawProperty(*Property, StringBuffer, _Input.GetObjectMutable());

        ImGui::PopID();
    }

    ImGui::EndTable();
}

void FKLDebugFeatureObject_PropertyWatcher::ImguiDrawProperty(FProperty& _Property, TArray<char>& _StringBuffer, UObject& _Object) const
{
    // based also on IteratePropertiesRecursive

    void* ContainerData = _Property.ContainerPtrToValuePtr<void*>(&_Object);

    if (const FClassProperty* ClassProp = CastField<const FClassProperty>(&_Property))
    {
        const UObject* Object = ClassProp->GetObjectPropertyValue(ContainerData);
        const UClass* Class = Object ? Cast<const UClass>(Object) : nullptr;
        if (Class)
        {
            ImGui::Text(TCHAR_TO_ANSI(*Class->GetAuthoredName()));
        }
        else
        {
            ImGui::Text("<Null>");
        }
    }
    else if (const FObjectProperty* ObjectProperty = CastField<const FObjectProperty>(&_Property))
    {
        const UObject* Object = ObjectProperty->GetObjectPropertyValue(ContainerData);
        if (Object)
        {
            ImGui::Text(TCHAR_TO_ANSI(*Object->GetName()));
        }
        else
        {
            ImGui::Text("<Null>");
        }
    }
    else if (const FClassPtrProperty* ClassPtrProp = CastField<const FClassPtrProperty>(&_Property))
    {
        ImGui::Text("PtrProperty not implemented");
    }
    else if (const FSoftClassProperty* SoftClassProp = CastField<const FSoftClassProperty>(&_Property))
    {
        const FSoftObjectPtr* SoftClass = static_cast<const FSoftObjectPtr*>(ContainerData);
        if (SoftClass->IsStale())
        {
            ImGui::Text("<Stale>");
        }
        else if (SoftClass->IsPending())
        {
            ImGui::Text("<Pending>");
        }
        else if (!SoftClass->IsValid())
        {
            ImGui::Text("<Null>");
        }
        else
        {
            const FString Path = SoftClass->ToSoftObjectPath().ToString();
            ImGui::Text(TCHAR_TO_ANSI(*Path));
        }
    }
    else if (const FWeakObjectProperty* WeakObjProp = CastField<const FWeakObjectProperty>(&_Property))
    {
        const TWeakObjectPtr<UObject>* WeakPtr = static_cast<const TWeakObjectPtr<UObject>*>(ContainerData);
        if (WeakPtr->IsStale())
        {
            ImGui::Text("<Stale>");
        }
        if (!WeakPtr->IsValid())
        {
            ImGui::Text("<Null>");
        }
        else
        {
            ImGui::Text(TCHAR_TO_ANSI(*((*WeakPtr)->GetName())));
        }
    }
    else if (const FLazyObjectProperty* LayzObjProp = CastField<const FLazyObjectProperty>(&_Property))
    {
        const TLazyObjectPtr<UObject>* LazyPtr = static_cast<const TLazyObjectPtr<UObject>*>(ContainerData);
        if (LazyPtr->IsStale())
        {
            ImGui::Text("<Stale>");
        }
        if (!LazyPtr->IsValid())
        {
            ImGui::Text("<Null>");
        }
        else
        {
            ImGui::Text(TCHAR_TO_ANSI(*((*LazyPtr)->GetName())));
        }
    }
    else if (const FSoftObjectProperty* SoftObjProp = CastField<const FSoftObjectProperty>(&_Property))
    {
        const TSoftObjectPtr<UObject>* SoftObjPtr = static_cast<const TSoftObjectPtr<UObject>*>(ContainerData);
        if (SoftObjPtr->IsPending())
        {
            ImGui::Text("<Pending>");
        }
        if (!SoftObjPtr->IsValid())
        {
            ImGui::Text("<Null>");
        }
        else
        {
            const FString Path = SoftObjPtr->ToSoftObjectPath().ToString();
            ImGui::Text(TCHAR_TO_ANSI(*Path));
        }
    }
    else if (CastField<const FByteProperty>(&_Property) && CastField<const FByteProperty>(&_Property)->IsEnum())
    {
        const FByteProperty& ByteProp = static_cast<const FByteProperty&>(_Property);
        if (!ByteProp.Enum)
        {
            return;
        }

        const uint8 Value = ByteProp.GetPropertyValue(ContainerData);
        const UEnum& Enum = *ByteProp.Enum;
        const int32 Count = Enum.NumEnums();

        _StringBuffer.Reset();
        for (int32 i = 0; i < Count; i++)
        {
            const FString Name = Enum.GetNameStringByIndex(i);
            _StringBuffer.Append(TCHAR_TO_ANSI(*Name), Name.Len());
            _StringBuffer.Emplace('\0');
        }

        _StringBuffer.Emplace('\0');
        int32 TempInt = Value;
        if (ImGui::Combo("##Enum", &TempInt, _StringBuffer.GetData(), Count))
        {
            ByteProp.SetPropertyValue(ContainerData, static_cast<uint8>(TempInt));
        }
    }
    else if (const FBoolProperty* BoolProp = CastField<const FBoolProperty>(&_Property))
    {
        bool TempBool = *static_cast<bool*>(ContainerData);
        ImGui::Checkbox("", &TempBool);
        BoolProp->SetPropertyValue(ContainerData, TempBool);
    }
    else if (_Property.IsA(FInt8Property::StaticClass()))
    {
        ImGui::InputScalar("##FInt8Property", ImGuiDataType_S8, ContainerData);
    }
    else if (_Property.IsA(FByteProperty::StaticClass()))
    {
        ImGui::InputScalar("##FByteProperty", ImGuiDataType_U8, ContainerData);
    }
    else if (_Property.IsA(FInt16Property::StaticClass()))
    {
        ImGui::InputScalar("##FInt16Property", ImGuiDataType_S16, ContainerData);
    }
    else if (_Property.IsA(FUInt16Property::StaticClass()))
    {
        ImGui::InputScalar("##FUInt16Property", ImGuiDataType_U16, ContainerData);
    }
    else if (_Property.IsA(FIntProperty::StaticClass()))
    {
        ImGui::InputScalar("##FIntProperty", ImGuiDataType_S32, ContainerData);
    }
    else if (_Property.IsA(FUInt32Property::StaticClass()))
    {
        ImGui::InputScalar("##FUInt32Property", ImGuiDataType_U32, ContainerData);
    }
    else if (_Property.IsA(FInt64Property::StaticClass()))
    {
        ImGui::InputScalar("##FInt64Property", ImGuiDataType_S64, ContainerData);
    }
    else if (_Property.IsA(FUInt64Property::StaticClass()))
    {
        ImGui::InputScalar("##FUInt64Property", ImGuiDataType_U64, ContainerData);
    }
    else if (_Property.IsA(FFloatProperty::StaticClass()))
    {
        ImGui::InputFloat("##FFloatProperty", static_cast<float*>(ContainerData));
    }
    else if (_Property.IsA(FDoubleProperty::StaticClass()))
    {
        ImGui::InputDouble("##FDoubleProperty", static_cast<double*>(ContainerData));
    }
    else if (_Property.IsA(FStrProperty::StaticClass()))
    {
        const FString Value = *static_cast<const FString*>(ContainerData);
        ImGui::Text(TCHAR_TO_ANSI(*Value));
    }
    else if (_Property.IsA(FNameProperty::StaticClass()))
    {
        const FString Value = (static_cast<const FName*>(ContainerData))->ToString();
        ImGui::Text(TCHAR_TO_ANSI(*Value));
    }
    else if (_Property.IsA(FTextProperty::StaticClass()))
    {
        const FString Value = (static_cast<const FText*>(ContainerData))->ToString();
        ImGui::Text(TCHAR_TO_ANSI(*Value));
    }
    else if (const FArrayProperty* ArrayProp = CastField<const FArrayProperty>(&_Property))
    {
        const FProperty* CurrentProp = ArrayProp->Inner;
        const FScriptArrayHelper ScriptArrayHelper(ArrayProp, ContainerData);
        ImGui::Text("Array %s [%d]", TCHAR_TO_ANSI(*CurrentProp->GetCPPType()), ScriptArrayHelper.Num());
    }
    else if (const FMapProperty* MapProp = CastField<const FMapProperty>(&_Property))
    {
        const FScriptMapHelper Helper(MapProp, ContainerData);
        ImGui::Text("Map <%s, %s> (%d)", TCHAR_TO_ANSI(*MapProp->KeyProp->GetCPPType()), TCHAR_TO_ANSI(*MapProp->ValueProp->GetCPPType()), Helper.Num());
    }
    else if (const FSetProperty* SetProp = CastField<const FSetProperty>(&_Property))
    {
        const FScriptSetHelper Helper(SetProp, ContainerData);
        ImGui::Text("Set <%s> {%d}", TCHAR_TO_ANSI(*Helper.GetElementProperty()->GetCPPType()), Helper.Num());
    }
    else if (const FMulticastDelegateProperty* MultiDelegateProp = CastField<const FMulticastDelegateProperty>(&_Property))
    {
        FString Text;
        if (const UFunction* SigFunction = MultiDelegateProp->SignatureFunction)
        {
            SigFunction->GetName(Text);
        }
        else
        {
            Text = "<No Function Bound>";
        }

        ImGui::Text(TCHAR_TO_ANSI(*Text));
    }
    else if (const FDelegateProperty* DelegateProp = CastField<const FDelegateProperty>(&_Property))
    {
        const FScriptDelegate* ScriptDelegate = static_cast<const FScriptDelegate*>(ContainerData);
        FString Text;
        if (ScriptDelegate->IsBound())
        {
            Text = ScriptDelegate->GetFunctionName().ToString();
        }
        else
        {
            Text = "<No Function Bound>";
        }

        ImGui::Text(TCHAR_TO_ANSI(*Text));
    }
    else if (const FMulticastInlineDelegateProperty* MultiInlineDelegateProp = CastField<const FMulticastInlineDelegateProperty>(&_Property))
    {
        // @Todo
    }
    else if (const FMulticastSparseDelegateProperty* MultiSparseDelegateProp = CastField<const FMulticastSparseDelegateProperty>(&_Property))
    {
        // @Todo
    }
    else if (const FStructProperty* StructProp = CastField<const FStructProperty>(&_Property))
    {
        FString Extended;
        FString StructType = StructProp->GetCPPType(&Extended, 0);

        if (StructType == "FVector")
        {
            ImGui::InputScalarN("##FVector", ImGuiDataType_Double, &(static_cast<FVector*>(ContainerData))->X, 3);
        }
        else if (StructType == "FTransform")
        {
            FTransform* Transform = static_cast<FTransform*>(ContainerData);
            FVector Location = Transform->GetLocation();
            FRotator Rotation = Transform->GetRotation().Rotator();
            FVector Scale = Transform->GetScale3D();

            if (ImGui::InputScalarN("##Location", ImGuiDataType_Double, &Location.X, 3))
            {
                Transform->SetLocation(Location);
            }

            if (ImGui::InputScalarN("##Scale", ImGuiDataType_Double, &Scale.X, 3))
            {
                Transform->SetScale3D(Scale);
            }

            if (ImGui::InputScalarN("##Rotation", ImGuiDataType_Double, &Rotation.Pitch, 3))
            {
                Transform->SetRotation(Rotation.Quaternion());
            }
        }
        else if (StructType == "FRotator")
        {
            ImGui::InputScalarN("##FRotator", ImGuiDataType_Double, &(static_cast<FRotator*>(ContainerData))->Pitch, 3);
        }
        else if (StructType == "FVector2D")
        {
            ImGui::InputScalarN("##FVector2D", ImGuiDataType_Double, &(static_cast<FVector2D*>(ContainerData))->X, 2);
        }
        else if (StructType == "FIntVector")
        {
            ImGui::InputInt3("##FIntVector", &(static_cast<FIntVector*>(ContainerData))->X);
        }
        else if (StructType == "FIntPoint")
        {
            ImGui::InputInt2("##FIntPoint", &(static_cast<FIntPoint*>(ContainerData))->X);
        }
        else if (StructType == "FTimespan")
        {
            const FString Value = static_cast<const FTimespan*>(ContainerData)->ToString();
            ImGui::Text(TCHAR_TO_ANSI(*Value));
        }
        else if (StructType == "FDateTime")
        {
            const FString Value = static_cast<const FTimespan*>(ContainerData)->ToString();
            ImGui::Text(TCHAR_TO_ANSI(*Value));
        }
        else if (StructType == "FLinearColor")
        {
            FLinearColor* LinearColor = static_cast<FLinearColor*>(ContainerData);
            FColor Color = LinearColor->ToFColor(true);
            float ColorArray[4] = { Color.R / 255.0f, Color.G / 255.0f, Color.B / 255.0f, Color.A / 255.0f };
            if (ImGui::ColorEdit4("##FLinearColor", ColorArray, ImGuiColorEditFlags_AlphaPreview))
            {
                Color = FColor(ColorArray[0] * 255.f, ColorArray[1] * 255.f, ColorArray[2] * 255.f, ColorArray[3] * 255.f);
                *LinearColor = FLinearColor::FromSRGBColor(Color);
            }
        }
        else if (StructType == "FColor")
        {
            FColor* Color = static_cast<FColor*>(ContainerData);
            float ColorArray[4] = { Color->R / 255.0f, Color->G / 255.0f, Color->B / 255.0f, Color->A / 255.0f };
            if (ImGui::ColorEdit4("##FColor", ColorArray, ImGuiColorEditFlags_AlphaPreview))
            {
                *Color = FColor(ColorArray[0] * 255.f, ColorArray[1] * 255.f, ColorArray[2] * 255.f, ColorArray[3] * 255.f);
            }
        }
        else
        {
            ImGui::Text("Struct type [%ls]", *StructType);
        }
    }
    else
    {
        ImGui::Text("<UnknownType>");
    }
}
