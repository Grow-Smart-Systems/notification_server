#ifndef DEFINES_H
#define DEFINES_H

#include <memory>

#include <QObject>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QVariant>
#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QDateTime>
#include <QHostAddress>
#include <QPair>
#include <QHash>
#include <QTextStream>
#include <QSet>
#include <QTimerEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QByteArray>
#include <QMap>

#include "Settings.h"
#include "Templates.h"
#include "GUID.h"

namespace Ethernet
{
    //! @brief Ключ для хэширования сокетов по адресу и порту
    using TcpSocketKey = QString;

    //! @brief Указатель на QTcpSocket
    using TcpSocketPtr = QSharedPointer<QTcpSocket>;

    //! @brief Хэш сокетов по <ключ подключения, сокет>
    using TcpSocketHash = QHash<TcpSocketKey, TcpSocketPtr>;

    //! @brief Указатель на QTcpServer
    using TcpServerPtr = std::unique_ptr<QTcpServer>;

    //! @brief Создает ключ для хэширования сокетов по адресу и порту
    //! @param address - адрес сокета
    //! @param port - порт сокета
    //! @return Ключ подключения
    static inline TcpSocketKey CreateTcpSocketKey(const QHostAddress& address, quint16 port)
    {
        return TcpSocketKey("%1:%2").arg(address.toString()).arg(port);
    }

    //! @brief Создает ключ для хэширования сокетов по QTcpSocket
    //! @param socket - указатель на QTcpSocket
    //! @return Ключ подключения
    static inline TcpSocketKey CreateTcpSocketKey(TcpSocketPtr socket)
    {
        return CreateTcpSocketKey(socket->peerAddress(), socket->peerPort());
    }

    //! @brief Парсинг ключа подключения из строки
    //! @return 
    static inline QPair<QHostAddress, quint16> ParseTcpSocketKey(const TcpSocketKey& key)
    {
        QStringList parts = key.split(':');
        if (parts.size() != 2)
            return QPair<QHostAddress, quint16>(QHostAddress(), 0);

        QHostAddress address = QHostAddress(parts[0]);
        quint16 port = static_cast<quint16>(parts[1].toUInt());
        return QPair<QHostAddress, quint16>(address, port);
    }
};

namespace Database
{
    //! @brief Указатель на QSqlQuery
    using DatabasePtr = QSharedPointer<QSqlDatabase>;

    //! @brief Пара <успешность выполнения запроса, список значений>
    using QueryResult = QPair<bool, QList<QVariant>>;
};


#endif // DEFINES_H