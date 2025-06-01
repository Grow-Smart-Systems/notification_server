#ifndef MESSAGEPING_H
#define MESSAGEPING_H

#include "Defines.h"
#include "BaseMessage.h"

namespace Logic::MessageFactory
{
    class MessagePing final : public BaseMessage
    {
    public:
        MessagePing(const Ethernet::TCPSocketKey& key, const Ethernet::HTTPPacket& packet)
            : BaseMessage(key, packet) {}

        ~MessagePing() = default;

    protected:
        void FormResponse() override;
    };
};

#endif // MESSAGEPING_H