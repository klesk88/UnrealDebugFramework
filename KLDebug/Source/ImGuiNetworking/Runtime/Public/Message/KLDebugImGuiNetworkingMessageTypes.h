#pragma once

enum class EKLDebugNetworkMessageTypes : uint8
{
    Client_FeatureStatusUpdate = 0,
    
    Server_FeatureDataUpdate,

    Count
};