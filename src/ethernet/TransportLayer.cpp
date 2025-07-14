#include "TransportLayer.h"
#include "../settings/Settings.h"

namespace Ethernet
{
    TransportLayer::TransportLayer(QObject* parent)
        : QObject{ parent }
    {
        _server = std::make_unique<QTcpServer>();
    }

    TransportLayer::~TransportLayer()
    {
        for (auto iter = _sockets.begin(); iter != _sockets.end(); ++iter)
        {
            iter.value()->deleteLater();
        }
        _sockets.clear();
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

    void TransportLayer::SendResponse(const TcpSocketKey& key, const QString& response)
    {
        auto socket = _sockets.value(key);
        if (!socket)
            return;
        
        QTextStream os(socket.get());
        os.setAutoDetectUnicode(true);
        os << response;
        os.flush();

        socket->close();
    }

    void TransportLayer::onNewConnection()
    {
        if (!_server->hasPendingConnections())
            return;

        auto newSocket = TcpSocketPtr::create(_server->nextPendingConnection());
        auto address {CreateTcpSocketKey(newSocket)};
        qInfo() << "(+) Connect from address: " << address;
        
        // Заполним хэш адреса и указателя на сокет
        _sockets.insert(address, newSocket);

        connect(newSocket.get(), SIGNAL(readyRead()), this, SLOT(onServerReadyRead()));
        connect(newSocket.get(), SIGNAL(disconnected()), this, SLOT(onDisconnection()));
    }

    void TransportLayer::onServerReadyRead()
    {
        if (QObject::sender() == nullptr)
            return;

        const auto sender = qobject_cast<QTcpSocket*>(QObject::sender());
        if (!sender)
            return;

        QByteArray array;
        while (sender->bytesAvailable() > 0)
        {
            array += sender->readAll();
        }
        if (array.isEmpty())
            return;

        auto keyConnection = CreateTcpSocketKey(sender->peerAddress(), sender->peerPort());
        if (!_sockets.contains(keyConnection))
        {
            qWarning() << "Socket not found for key: " << keyConnection;
            return;
        }

        emit signalNewMessageReceived(keyConnection, array);
    }

    void TransportLayer::onDisconnection()
    {
        auto* sender = qobject_cast<QTcpSocket*>(QObject::sender());
        if (!sender)
            return;

        auto keyConnection = CreateTcpSocketKey(sender->peerAddress(), 
                                                sender->peerPort());
        if (!_sockets.contains(keyConnection))
        {
            qWarning() << "Socket not found for key: " << keyConnection;
            return;
        }

        auto socket = _sockets.value(keyConnection);
        if (!socket)
        {
            qWarning() << "Socket pointer is null for key: " << keyConnection;
            return;
        }

        disconnect(socket.get(), SIGNAL(readyRead()), this, SLOT(onServerReadyRead()));
        disconnect(socket.get(), SIGNAL(disconnected()), this, SLOT(onDisconnection()));

        qInfo() << "(-) Disconnection from address: " << socket->peerAddress().toString();

        socket->deleteLater();
        _sockets.remove(keyConnection);
    }
};
