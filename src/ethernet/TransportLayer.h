#ifndef TRANSPORTLAYER_H
#define TRANSPORTLAYER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QDateTime>


class TransportLayer final : public QObject
{
    Q_OBJECT

    using TCPSocketPtr = QTcpSocket*;
    using ListTCPSocketPtr = QHash<QHostAddress, TCPSocketPtr>;
    using QTcpServerPtr = QSharedPointer<QTcpServer>;

public:
    //! \brief Конструктор класса транспортного уровеня
    explicit TransportLayer(QObject *parent = nullptr);

    ~TransportLayer();

    //! \brief Инициализация класса транспортного уровня
    bool init();

protected slots:
    //! \brief На новое подключение
    void onNewConnection();

    //! \brief Слот на получение сообщения
    void onServerReadyRead();

    //! \brief Слот на отключение сокета
    void onDisconnection();

private:
    //! \brief Указатель на сервер
    QSharedPointer<QTcpServer> _server;

    //! \brief Лист сокетов для обработки входящих сообщений
    ListTCPSocketPtr _socketList;
signals:
    //! \brief Сигнал получения сообщения
    void signalNewMessageReceived(QString);
};

#endif // TRANSPORTLAYER_H
