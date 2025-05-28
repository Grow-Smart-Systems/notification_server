#include "RequestRouter.h"

RequestRouter::RequestRouter(QObject* parent)
    : QObject(parent)
{
    _applicationLayer = QSharedPointer<ApplicationLayer>::create();
    if(!_applicationLayer->Init())
    {
        qCritical() << "Failed to initialize ApplicationLayer";
        return;
    }
    connect(_applicationLayer.data(), &ApplicationLayer::signalNewRequest, 
        this, &RequestRouter::OnNewRequest);
}

void RequestRouter::OnNewRequest(const HTTPPacket& packet)
{
    QString response = "RequestRouter::Received request: " + packet.RequestString();
    qDebug() << response;
}