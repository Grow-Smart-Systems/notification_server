#include "TransportLayer.h"
#include "../settings/Settings.h"

namespace Ethernet
{
    TransportLayer::TransportLayer(QObject* parent)
        : QObject{ parent }
    {
        _server = QSharedPointer<QTcpServer>::create();
    }

    TransportLayer::~TransportLayer()
    {
        for (auto iter = _socketHash.begin();iter != _socketHash.end(); ++iter)
        {
            iter.value()->deleteLater();
        }
        _socketHash.clear();
    }

    bool TransportLayer::init()
    {
        Settings* settings = Settings::GetInstance();
        if (_server->listen(QHostAddress::Any, settings->GetListenPort()))
        {
            qInfo() << "(!) Server listen - address: " << _server->serverAddress().toString() << " port: " << _server->serverPort();
            connect(_server.get(), SIGNAL(newConnection()), this, SLOT(onNewConnection()));
        }
        else
        {
            qCritical() << "Cannot start server: " << _server->errorString();
            return false;
        }

        return true;
    }

    void TransportLayer::SendResponse(const TCPSocketKey& key, const QString& response)
    {
        auto* socket = _socketHash.value(key);
        if (!socket)
            return;
        
        QTextStream os(socket);
        os.setAutoDetectUnicode(true);
        os << _responseTemplate.arg(response);
        os.flush();

        socket->close();
    }

    void TransportLayer::onNewConnection()
    {
        if (!_server->hasPendingConnections())
            return;

        TCPSocketPtr newSocket(_server->nextPendingConnection());
        TCPSocketKey keyConnection{ newSocket->peerAddress(), newSocket->peerPort() };
        if (_socketHash.contains(keyConnection))
            return;

        qInfo() << "(+) Connect from address: " << newSocket->peerAddress().toString();

        connect(newSocket, SIGNAL(readyRead()), this, SLOT(onServerReadyRead()));
        connect(newSocket, SIGNAL(disconnected()), this, SLOT(onDisconnection()));

        _socketHash.insert(keyConnection, newSocket);
    }

    void TransportLayer::onServerReadyRead()
    {
        if (QObject::sender() == nullptr)
            return;

        const auto sender = qobject_cast<QTcpSocket*>(QObject::sender());
        if (!sender)
            return;

        QString text;
        while (sender->bytesAvailable() > 0)
        {
            text += QString::fromUtf8(sender->readAll());
        }

        TCPSocketKey keyConnection{ sender->peerAddress(), sender->peerPort() };
        emit signalNewMessageReceived(keyConnection, text);
    }

    void TransportLayer::onDisconnection()
    {
        auto address = ((QTcpSocket*)QObject::sender())->peerAddress();
        auto* sender = qobject_cast<QTcpSocket*>(QObject::sender());
        if (!sender)
            return;

        TCPSocketKey keyConnection{ sender->peerAddress(), sender->peerPort() };

        auto socket = _socketHash.value(keyConnection);

        disconnect(socket, SIGNAL(readyRead()), this, SLOT(onServerReadyRead()));
        disconnect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnection()));

        qInfo() << "(-) Disconnection from address: " << socket->peerAddress().toString();

        _socketHash.value(keyConnection)->deleteLater();
        _socketHash.remove(keyConnection);
    }
};
