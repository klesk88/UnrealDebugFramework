// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Mode/KLDebugExampleMode.h"

// modules
#include "ImGui/User/Public/Helpers/KLDebugImGuiHelpers.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"
#include "ThirdParty/ImPlotThirdParty/Public/Library/implot.h"
#include "User/Framework/Public/Mode/KLDebugModeInterfaceAllInput.h"
#include "User/Networking/Public/Window/KLDebugNetworkingWindowAllInputs.h"

// engine
#include "Camera/PlayerCameraManager.h"
#include "Engine/DebugCameraController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshPath.h"
#include "NavMesh/NavMeshRenderingComponent.h"
#include "NavMesh/RecastNavMesh.h"

KL_DEBUG_CREATE_MODE(FKLDebugExampleMode)

void FKLDebugExampleMode::OnSelect(const UWorld& _World)
{
    if (!ImPlotExampleXFirst.IsEmpty())
    {
        return;
    }

    // based on Demo_LinePlots in implot_demp.cpp

    static constexpr int32 FirstSize = 1001;
    static constexpr int32 SecondSize = 20;

    ImPlotExampleXFirst.Reserve(1001);
    ImPlotExampleYFirst.Reserve(1001);
    ImPlotExampleXSecond.Reserve(20);
    ImPlotExampleYSecond.Reserve(20);

    for (int32 i = 0; i < FirstSize; ++i)
    {
        ImPlotExampleXFirst.Emplace(i * 0.001f);
        ImPlotExampleYFirst.Emplace(0.5f + 0.5f * sinf(50.f * (ImPlotExampleXFirst[i] + ImGui::GetTime() / 10.f)));
    }

    for (int32 i = 0; i < SecondSize; ++i)
    {
        ImPlotExampleXSecond.Emplace(i * 1 / 19.f);
        ImPlotExampleYSecond.Emplace(ImPlotExampleXSecond[i] * ImPlotExampleXSecond[i]);
    }
}

const FString& FKLDebugExampleMode::GetFriendlyName() const
{
    static FString Name(TEXT("ExampleMode"));
    return Name;
}

TUniquePtr<IKLDebugContextInterface> FKLDebugExampleMode::GetContext(const FKLDebugContextGetterInput& _Input) const
{
    if (_Input.GetCurrentNetMode() == ENetMode::NM_Client)
    {
        return MakeUnique<FDebugExampleModeClientContext>();
    }
    else if (_Input.GetCurrentNetMode() != ENetMode::NM_Standalone)
    {
        return MakeUnique<FDebugExampleModeServerContext>();
    }

    return nullptr;
}

void FKLDebugExampleMode::DrawImGui(const FKLDebugModeDrawImGuiInput& _Input)
{
    ImGuiViewport* Viewport = ImGui::GetMainViewport();
    ImDrawList* DrawList = ImGui::GetForegroundDrawList();
    DrawList->AddCircle(Viewport->GetCenter(), 10.f, ImColor(1.0f, 0.0f, 0.0f), 10, 2.f);

    ImGui::SetCursorPosY(200.f);
    ImGui::Text("example imgui text");

    if (_Input.GetWorld().GetNetMode() == ENetMode::NM_Client)
    {
        const FDebugExampleModeClientContext& Context = _Input.GetContext<FDebugExampleModeClientContext>();

        ImGui::Text(" Network Nav Tile X: [%d]", Context.mSyncedCurrentTileX);
        ImGui::Text(" Network Nav Tile Y: [%d]", Context.mSyncedCurrentTileY);
        ImGui::Text(" Network Nav Tile Count: [%d]", Context.mRequestServerTileCount);
    }

    ImGui::PushItemWidth(400.f);
    ImGui::InputFloat("Plot X Size", &mPlotXSize, 30.f);
    ImGui::InputFloat("Plot Y Size", &mPlotYSize, 30.f);
    ImGui::PopItemWidth();

    // based on Demo_LinePlots in implot_demp.cpp
    if (ImPlot::BeginPlot("Line Plots", ImVec2(mPlotXSize, mPlotYSize)))
    {
        ImPlot::SetupAxes("x", "y");
        ImPlot::PlotLine("f(x)", ImPlotExampleXFirst.GetData(), ImPlotExampleYFirst.GetData(), ImPlotExampleXFirst.Num());
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::PlotLine("g(x)", ImPlotExampleXSecond.GetData(), ImPlotExampleYSecond.GetData(), ImPlotExampleXSecond.Num(), ImPlotLineFlags_Segments);
        ImPlot::EndPlot();
    }
}

void FKLDebugExampleMode::DrawCanvas(FKLDebugModeDrawCanvasInput& _Input) const
{
    _Input.Printf(TEXT("Example Canvas text: {%s}%d"), TEXT("red"), 1);
}

void FKLDebugExampleMode::Server_Tick(const FKLDebugNetworkingWindowServerTickInput& _Input)
{
    const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(&_Input.GetWorld());
    const ANavigationData* NavData = NavSys ? NavSys->GetDefaultNavDataInstance() : nullptr;
    const ARecastNavMesh* RecastNavMesh = NavData ? Cast<const ARecastNavMesh>(NavData) : nullptr;
    if (!RecastNavMesh)
    {
        return;
    }

    const UWorld& World = _Input.GetWorld();
    const APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(&World, 0);
    if (!PlayerCamera)
    {
        return;
    }

    FVector Location = FVector::ZeroVector;
    FRotator CamRot = FRotator::ZeroRotator;
    PlayerCamera->GetCameraViewPoint(Location, CamRot);

    int32 TargetTileX = 0;
    int32 TargetTileY = 0;
    RecastNavMesh->GetNavMeshTileXY(Location, TargetTileX, TargetTileY);

    FDebugExampleModeServerContext& Context = _Input.GetContextMutable<FDebugExampleModeServerContext>();
    if (Context.mSyncedCurrentTileX == TargetTileX && Context.mSyncedCurrentTileY == TargetTileY)
    {
        return;
    }

    Context.mSyncedCurrentTileX = TargetTileX;
    Context.mSyncedCurrentTileY = TargetTileY;

    FArchive& Archive = _Input.GetArchiveMutable();
    Archive << TargetTileY;
    Archive << TargetTileX;
}

void FKLDebugExampleMode::Server_FeatureUpdate(const FKLDebugNetworkingWindowRequestUpdateInput& _Input) const
{
    const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(&_Input.GetWorld());
    const ANavigationData* NavData = NavSys ? NavSys->GetDefaultNavDataInstance() : nullptr;
    const ARecastNavMesh* RecastNavMesh = NavData ? Cast<const ARecastNavMesh>(NavData) : nullptr;
    if (!RecastNavMesh)
    {
        return;
    }

    int32 TilesNum = RecastNavMesh->GetNavMeshTilesCount();
    _Input.GetWriter() << TilesNum;
}

void FKLDebugExampleMode::Client_ReceiveData(const FKLDebugNetworkingWindowReceiveDataInput& _Input)
{
    FDebugExampleModeClientContext& Context = _Input.GetContextMutable<FDebugExampleModeClientContext>();
    switch (_Input.GetReceiveMessageType())
    {
    case EKLDebugImGuiNetworkReceiveMessageType::RequestUpdate:
        _Input.GetArchiveMutable() << Context.mRequestServerTileCount;
        break;
    case EKLDebugImGuiNetworkReceiveMessageType::ServerTick:
        _Input.GetArchiveMutable() << Context.mSyncedCurrentTileX;
        _Input.GetArchiveMutable() << Context.mSyncedCurrentTileY;
        break;
    }
}

void FKLDebugExampleMode::Client_Tick(FKLDebugNetworkingWindowClientTickInput& _Input)
{
    bool Update = false;
    _Input.GetArchiveMutable() << Update;
}
