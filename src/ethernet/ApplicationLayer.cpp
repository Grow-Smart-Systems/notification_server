#include "ApplicationLayer.h"

namespace Ethernet
{
    ApplicationLayer::ApplicationLayer(QObject* parent)
        : QObject{ parent }
    {
        _transportLayer = QSharedPointer<TransportLayer>::create();
    }

    bool ApplicationLayer::Init()
    {
        connect(_transportLayer.get(), &TransportLayer::signalNewMessageReceived,
            this, &ApplicationLayer::OnNewMessage);

        return _transportLayer->init();
    }

    void ApplicationLayer::SendResponse(const TCPSocketKey& key, const QString& response)
    {
        int timerId = _waitingRequests.key(key);
        if (timerId == 0)
        {
            qWarning() << "No request found for the given key, cannot send response";
            return;
        }
        killTimer(timerId); // Остановить таймер, если он существует
        _waitingRequests.remove(timerId); // Удалить из списка ожидающих запросов
        _transportLayer->SendResponse(key, response);
    }

    void ApplicationLayer::timerEvent(QTimerEvent* event)
    {
        int timerId = event->timerId();
        if (_waitingRequests.contains(timerId))
        {
            killTimer(timerId); // Остановить таймер, если он существует
            const auto& key = _waitingRequests.value(timerId);
            qWarning() << "Request timed out, no response received";
            _transportLayer->SendResponse(key, "HTTP/1.1 408 Request Timeout\r\n\r\n");
            _waitingRequests.remove(timerId);
        }
    }

    void ApplicationLayer::OnNewMessage(const TCPSocketKey& key, const QString& msg)
    {
        HTTPPacket httpPacket(msg);
        if (!httpPacket.Parse())
        {
            // Если не удалось разобрать HTTP-запрос, выводим сообщение об ошибке
            _transportLayer->SendResponse(key, "HTTP/1.1 400 Bad Request\r\n\r\n");
            return;
        }

        _waitingRequests.insert(startTimer(MAX_WAITING_TIME), key);

        emit signalNewRequest(httpPacket);
    }
};
