#include "RequestRouter.h"

RequestRouter::RequestRouter(QObject* parent)
    : QObject(parent)
{
    _applicationLayer = QSharedPointer<Ethernet::ApplicationLayer>::create();
    if(!_applicationLayer->Init())
    {
        qCritical() << "Failed to initialize ApplicationLayer";
        return;
    }
    connect(_applicationLayer.data(), &Ethernet::ApplicationLayer::signalNewRequest, 
        this, &RequestRouter::OnNewRequest);
}

void RequestRouter::OnNewRequest(const Ethernet::HTTPPacket& packet)
{
    QString response = "RequestRouter::Received request: " + packet.RequestString();
    qDebug() << response;
}