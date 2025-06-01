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
        //! @param key Ключ сокета, по которому будет отправлен ответ.
        //! @param response Ответ в виде строки, который будет отправлен клиенту.
        void SendResponse(const TCPSocketKey& key, const QString& response);

    protected slots:
        //! @brief Обрабатывает получение нового сообщения от транспортного уровня.
        //! @param key Ключ сокета
        //! @param msg Сообщение, полученное от транспортного уровня, в виде QString.
        void OnNewMessage(const TCPSocketKey& key, const QString& msg);

        //! @brief Обрабатывает событие таймера для управления ожиданием ответов.
        //! @param event Событие таймера, которое содержит идентификатор таймера.
        void timerEvent(QTimerEvent* event) override;

    private:
        //! @brief Указатель на транспортный уровень
        QSharedPointer<TransportLayer> _transportLayer;

        //! @brief Функция выделения запроса из всего сообщения
        QString getRequestFromMessage(QString message);

        //! @brief Набор идентификаторов запросов, которые ожидают ответа
        QMap<int, TCPSocketKey> _waitingRequests;

    signals:
        //! @brief Сигнал, который отправляется при получении нового HTTP-запроса.
        //! @param packet Объект HTTPPacket, содержащий разобранный HTTP-запрос.
        void signalNewRequest(const TCPSocketKey&, const HTTPPacket&);
    };
};

#endif // APPLICATIONLAYER_H
