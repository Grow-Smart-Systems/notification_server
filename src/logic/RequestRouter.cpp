#include "RequestRouter.h"
#include "message_processor/MessageFactory.h"

namespace Logic
{
    RequestRouter::RequestRouter(QObject* parent)
        : QObject(parent)
    {
        _applicationLayer = QSharedPointer<Ethernet::ApplicationLayer>::create();
        if (!_applicationLayer->Init())
        {
            qCritical() << "Failed to initialize ApplicationLayer";
            return;
        }
        connect(_applicationLayer.data(), &Ethernet::ApplicationLayer::signalNewRequest,
            this, &RequestRouter::OnNewRequest);
    }

    void RequestRouter::OnNewRequest(const Ethernet::TCPSocketKey& key, const Ethernet::HTTPPacket& packet)
    {
        // Использование фабрики для создания сообщения
        auto message = Logic::MessageFactory::MessageFactory::parseMessage(key, packet);
        if (!message) 
            return;

        connect(message.get(), &Logic::MessageFactory::BaseMessage::SendResponse,
            _applicationLayer.data(), &Ethernet::ApplicationLayer::SendResponse);

        message->HandleMassage();
    }
} // namespace Logic

