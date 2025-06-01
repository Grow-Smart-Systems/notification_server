#include "MessageFactory.h"

#include "Messages.h"

namespace Logic::MessageFactory
{
    std::unique_ptr<BaseMessage> MessageFactory::parseMessage(const Ethernet::TCPSocketKey& key, 
        const Ethernet::HTTPPacket& packet)
    {
        const auto path = packet.RequestStringPath().toLower();
        qDebug() << "Parsing message:" << path;

        if(path == "/ping")
            return std::make_unique<MessagePing>(key, packet);

        return nullptr;
    }
}; // namespace Logic