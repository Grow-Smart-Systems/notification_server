#include "MessageFactory.h"

#include "Messages.h"

namespace Logic::MessageFactory
{
    std::shared_ptr<BaseMessage> MessageFactory::parseMessage(const GUID& deviceGuid, 
        const Ethernet::HTTPPacket& packet)
    {
        const auto path = packet.RequestStringPath().toLower();
        qDebug() << "Parsing message:" << path;

        if(path == "/ping")
            return std::make_shared<MessagePing>(deviceGuid, packet);

        return nullptr;
    }
}; // namespace Logic