#ifndef TRANSPORTLAYER_H
#define TRANSPORTLAYER_H

#include "Defines.h"


namespace Ethernet
{
    class TransportLayer final : public QObject
    {
        Q_OBJECT
        
    public:
        //! @brief Конструктор класса транспортного уровня
        explicit TransportLayer(QObject* parent = nullptr);

        //! @brief Деструктор класса транспортного уровня
        ~TransportLayer();

        //! @brief Инициализация класса транспортного уровня
        bool init();

        //! @brief Слот на отправку ответа клиенту
        void SendResponse(const TcpSocketKey& address, const QString& response);

    protected slots:
        //! @brief На новое подключение
        void onNewConnection();

        //! @brief Слот на получение сообщения
        void onServerReadyRead();

        //! @brief Слот на отключение сокета
        void onDisconnection();

    private:
        //! @brief Указатель на сервер
        TcpServerPtr _server;

        //! @brief Хэш сокетов
        //! @details Ключ - адрес:порт, значение - указатель на QTcpSocket
        TcpSocketHash _sockets;

    signals:
        //! @brief Сигнал получения сообщения
        //! @param key Ключ сокета, по которому было получено сообщение
        //! @param data Данные сообщения
        void signalNewMessageReceived(const TcpSocketKey&, const QByteArray&);
    };
};

#endif // TRANSPORTLAYER_H
