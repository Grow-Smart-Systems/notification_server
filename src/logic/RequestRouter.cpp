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
            this, &RequestRouter::OnNewRequest, Qt::QueuedConnection);
    }

    void RequestRouter::OnNewRequest(const GUID& deviceGuid, const Ethernet::HTTPPacket& packet)
    {
        auto message = Logic::MessageFactory::MessageFactory::parseMessage(deviceGuid, packet);
        if (!message) 
            return;

        connect(message.get(), &Logic::MessageFactory::BaseMessage::SendResponse,
            _applicationLayer.data(), &Ethernet::ApplicationLayer::SendResponse);

        // Запускаем обработку сообщения в отдельном потоке
        auto future = QtConcurrent::run([message = std::move(message)]() mutable 
        {
            message->HandleMessage();
        });
    }
} // namespace Logic

