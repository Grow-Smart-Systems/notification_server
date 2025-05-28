#ifndef TRANSPORTLAYER_H
#define TRANSPORTLAYER_H

#include "Defines.h"


namespace Ethernet
{
    class TransportLayer final : public QObject
    {
        Q_OBJECT
        
    public:
        //! \brief Конструктор класса транспортного уровеня
        explicit TransportLayer(QObject* parent = nullptr);

        //! \brief Деструктор класса транспортного уровня
        ~TransportLayer();

        //! \brief Инициализация класса транспортного уровня
        bool init();

        //! \brief Слот на отправку ответа клиенту
        void SendResponse(const TCPSocketKey& key, const QString& response);

    protected slots:
        //! \brief На новое подключение
        void onNewConnection();

        //! \brief Слот на получение сообщения
        void onServerReadyRead();

        //! \brief Слот на отключение сокета
        void onDisconnection();

    private:
        //! \brief Указатель на сервер
        TcpServerPtr _server;

        //! \brief Хэш сокетов для обработки входящих сообщений.
        //! \remarks Ключ - пара <адрес клиента, порт>, значение - указатель на QTcpSocket.
        TCPSocketHash _socketHash;

        const QString _responseTemplate {
            "HTTP/1.1 200 Ok\r\n"
            "Content-Type: text/html; charset=\"utf-8\"\r\n"
            "\r\n"
            "%1\n"
        };

    signals:
        //! \brief Сигнал получения сообщения
        void signalNewMessageReceived(const TCPSocketKey&, const QString&);
    };
};

#endif // TRANSPORTLAYER_H
