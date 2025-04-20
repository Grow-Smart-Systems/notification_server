#include "ApplicationLayer.h"

ApplicationLayer::ApplicationLayer(QObject *parent)
    : QObject{parent}
{
    _transportLayer = QSharedPointer<TransportLayer>::create();
}

bool ApplicationLayer::init()
{
    connect(_transportLayer.get(), SIGNAL(signalNewMessageReceived(QString)), this, SLOT(onNewMessage(QString)));

    return _transportLayer->init();
}

void ApplicationLayer::onNewMessage(QString msg)
{
    //NOTE: http get filter
    if(!msg.contains("GET", Qt::CaseInsensitive))
        return;

    HTTPPacket httpPacket(msg);
    if(httpPacket.parse())
        emit signalNewRequest(httpPacket);
    else
        qInfo() << "parse message unsuccesfull";
}
