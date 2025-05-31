#ifndef DEFINES_H
#define DEFINES_H

#include <memory>

#include <QObject>
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

namespace Ethernet
{
    using TCPSocketPtr = QTcpSocket*;
    using TCPSocketKey = QPair<QHostAddress, qint16>;
    using TCPSocketHash = QHash<TCPSocketKey, TCPSocketPtr>;
    using TcpServerPtr = QSharedPointer<QTcpServer>;
}


#endif // DEFINES_H