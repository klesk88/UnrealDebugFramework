#include "Message/KLDebugImGuiNetworkingMessageTypes.h"

//engine
#include "Math/NumericLimits.h"

namespace KL::Debug::ImGui::Networking::Message
{
    static MessageID CurrentMessageID = 0;

    MessageID GetNewMessageID()
    {
        CurrentMessageID = (static_cast<uint32>(CurrentMessageID) + 1) % TNumericLimits<MessageID>::Max();
        return CurrentMessageID;
    }
}