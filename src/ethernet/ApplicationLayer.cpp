#include "ApplicationLayer.h"
#include "Crypto.h"

namespace Ethernet
{
    ApplicationLayer::ApplicationLayer(QObject* parent)
        : QObject{ parent }
    {
        _transportLayer = std::make_unique<TransportLayer>();
        _enableCryptography = Settings::GetInstance()->GetEnableCrypto();
    }

    bool ApplicationLayer::Init()
    {
        connect(_transportLayer.get(), &TransportLayer::signalNewMessageReceived,
                this, &ApplicationLayer::OnNewMessage);

        return _transportLayer->init();
    }

    void ApplicationLayer::SendResponse(const GUID& guid, const QString& response)
    {
        auto key = _addressCache.value(guid);
        if (key.isEmpty())
            return;

        int timerId = _waitingRequests.key(key);
        if (timerId == 0)
        {
            qWarning() << "No request found for the given key, cannot send response";
            return;
        }

        // Остановить таймер таймаута ожидания ответа
        killTimer(timerId);

        // Удалить из списка ожидающих запросов
        _waitingRequests.remove(timerId);

        // Если включена криптография, шифруем ответ
        if (_enableCryptography)
            _transportLayer->SendResponse(key, Crypto::Crypto::getInstance().EncryptMessage(guid, response));
        else
            _transportLayer->SendResponse(key, response);
    }

    bool ApplicationLayer::processGUID(const TcpSocketKey& key, const GUID& guid)
    {
        if (!guid.IsValid())
            return false;

        if (!_addressCache.contains(guid) ||
            (_addressCache.contains(guid) && _addressCache.value(guid) != key))
        {
            _addressCache.insert(guid, key);
            emit signalNewDeviceAddress(key, guid);
        }

        return true;
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

    void ApplicationLayer::OnNewMessage(const TcpSocketKey& key, const QByteArray& msg)
    {
        // Проверяем, что сообщение не пустое
        if (msg.isEmpty())
            return;

        // Если криптография включена, проверяем и расшифровываем сообщение
        QString message;
        GUID guid;
        if (_enableCryptography)
        {
            auto result = Crypto::Crypto::getInstance().DecryptMessage(msg);

            // Обработка результата расшифровки
            guid = GUID{result.first};
            if (!processGUID(key, guid))
            {
                _transportLayer->SendResponse(key, "HTTP/1.1 403 Forbidden\r\n\r\n");
                return;
            }

            message = result.second;
            if (message.isEmpty())
            {
                _transportLayer->SendResponse(key, "HTTP/1.1 500 Internal Server Error\r\n\r\n");
                return;
            }
        }
        else
        {
            //TODO получть GUID из кэша или базы по ключу подключения
            // !!! Только для отладки. Не использование шифрования ошибочно по логике формирования сообщений !!!
            auto cachedGuid = _addressCache.key(key);
            if (!cachedGuid.IsNull())
                guid = cachedGuid;
            else
            {
                // Загрузим из БД guid по ключу (адрес+порт)
            }

            message = QString::fromUtf8(msg);
        }

        // Парсинг сообщения
        HTTPPacket httpPacket(message);
        if (!httpPacket.Parse())
        {
            _transportLayer->SendResponse(key, "HTTP/1.1 400 Bad Request\r\n\r\n");
            return;
        }

        _waitingRequests.insert(startTimer(MAX_WAITING_TIME), key);

        emit signalNewRequest(guid, httpPacket);
    }
};
