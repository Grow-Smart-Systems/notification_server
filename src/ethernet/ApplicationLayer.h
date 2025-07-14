#ifndef APPLICATIONLAYER_H
#define APPLICATIONLAYER_H

#include "TransportLayer.h"
#include "HTTPPacket.h"


namespace Ethernet
{
    class ApplicationLayer final : public QObject
    {
        Q_OBJECT

        //! \brief Максимальное время ожидания ответа
        static constexpr int MAX_WAITING_TIME = 10000;

    public:
        //! @brief Конструктор класса прикладного уровня получения
        explicit ApplicationLayer(QObject* parent = nullptr);

        //! @brief Инициализирует экземпляр ApplicationLayer.
        //! @return Возвращает true, если инициализация прошла успешно, иначе false.
        bool Init();

        //! @brief Отправляет ответ клиенту по транспортному уровню.
        //! @param guid GUID устройства, для которого будет отправлен ответ.
        //! @param response Ответ в виде строки, который будет отправлен клиенту.
        void SendResponse(const GUID& guid, const QString& response);

    protected:
        //! @brief Обрабатывает получение GUID из сообщения.
        //! @param key Ключ сокета, по которому было получено сообщение.
        //! @param guid GUID, полученная из сообщения.
        //! @return Возвращает true, если GUID был успешно обработан, иначе false.
        bool processGUID(const TcpSocketKey& key, const GUID& guid);

    protected slots:
        //! @brief Обрабатывает получение нового сообщения от транспортного уровня.
        //! @param key Ключ сокета
        //! @param msg Сообщение, полученное от транспортного уровня, в виде QByteArray.
        void OnNewMessage(const TcpSocketKey& key, const QByteArray& msg);

        //! @brief Обрабатывает событие таймера для управления ожиданием ответов.
        //! @param event Событие таймера, которое содержит идентификатор таймера.
        void timerEvent(QTimerEvent* event) override;

    private:
        //! @brief Указатель на транспортный уровень
        std::unique_ptr<TransportLayer> _transportLayer {nullptr};

        //! @brief Набор идентификаторов запросов, которые ожидают ответа
        QMap<int, TcpSocketKey> _waitingRequests;

        //! @brief Хэш сокетов, которые уже были обработаны
        //! @details Ключ - адрес:порт, значение - GUID
        QHash<GUID, TcpSocketKey> _addressCache;

        //! @brief Флаг, указывающий, включена ли криптография
        //! @details Если true, то сообщения будут проверяться и расшифровываться.
        bool _enableCryptography {false};

    signals:
        //! @brief Сигнал, который отправляется при получении нового HTTP-запроса.
        //! @param guid GUID устройства, отправившего запрос.
        //! @param packet Объект HTTPPacket, содержащий разобранный HTTP-запрос.
        void signalNewRequest(const GUID&, const HTTPPacket&);

        //! @brief Сигнал, который отправляется при получении нового GUID.
        //! @param key Ключ сокета, по которому был получен GUID.
        //! @param guid GUID устройства, полученный из сообщения.
        void signalNewDeviceAddress(const TcpSocketKey& key, const GUID& guid);
    };
};

#endif // APPLICATIONLAYER_H
