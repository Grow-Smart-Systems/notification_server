#include "TransportLayer.h"
#include "../settings/Settings.h"

TransportLayer::TransportLayer(QObject *parent)
    : QObject{parent}
{
    _server = QSharedPointer<QTcpServer>::create();
}

TransportLayer::~TransportLayer()
{
    for(auto iter =_socketList.begin();iter != _socketList.end(); ++iter )
    {
        iter.value()->deleteLater();
    }
    _socketList.clear();
}

bool TransportLayer::init()
{
    Settings* settings = Settings::getInstance();
    if(_server->listen(QHostAddress::Any, settings->getListenPort()))
    {
        qInfo() << "(!) Server listen - address: " << _server->serverAddress().toString() << " port: " << _server->serverPort();
        connect(_server.get(), SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    }
    else
    {
        qCritical()<< "Cannot start server: " << _server->errorString();
        return false;
    }

    return true;
}

void TransportLayer::onNewConnection()
{
    if(_server->hasPendingConnections())
    {
        TCPSocketPtr newSocket(_server->nextPendingConnection());

        if(!_socketList.contains(newSocket->peerAddress()))
        {
            qInfo() << "(+) Connect from address: " << newSocket->peerAddress().toString();

            connect(newSocket, SIGNAL(readyRead()), this, SLOT(onServerReadyRead()));
            connect(newSocket, SIGNAL(disconnected()), this, SLOT(onDisconnection()));

            _socketList.insert(newSocket->peerAddress(), newSocket);
        }
    }
}

void TransportLayer::onServerReadyRead()
{
    if(QObject::sender() == nullptr)
        return;

    const auto sender = qobject_cast<QTcpSocket*>(QObject::sender());

    QTextStream os(sender);
    os.setAutoDetectUnicode(true);

    QString text;
    while(sender->bytesAvailable() > 0)
    {
         text += QString::fromUtf8(sender->readAll());
    }

    os << "HTTP/1.1 200 Ok\r\n"
          "Content-Type: text/html; charset=\"utf-8\"\r\n"
          "\r\n"
          "<h1>Nothing to see here</h1>\n"
       << QDateTime::currentDateTime().toString();

    sender->close();

    emit signalNewMessageReceived(text);
}

void TransportLayer::onDisconnection()
{
     auto address = ((QTcpSocket*)QObject::sender())->peerAddress();
     auto socket = _socketList.value(address);

     disconnect(socket, SIGNAL(readyRead()), this, SLOT(onServerReadyRead()));
     disconnect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnection()));

     qInfo() << "(-) Disconnection from address: " << socket->peerAddress().toString();

     _socketList.value(address)->deleteLater();
     _socketList.remove(address);
}
