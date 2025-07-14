#include "MessagePing.h"

namespace Logic::MessageFactory
{
    void MessagePing::FormResponse()
    {
        // Формируем ответ для клиента
        QString response  {"HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html; charset=utf-8\r\n"
                           "Connection: close\r\n"
                           "\r\n"
                           "<h1>pong</h1>\r\n"
                           "<p>Current server time: %1 </p>\r\n"};
        auto serverTime = QDateTime::currentDateTime().currentSecsSinceEpoch();
        response = response.arg(serverTime);
        if(auto clientTime = _packet.RequestParameters().value("time"); !clientTime.isEmpty())
        {
            response += "<p>delta: " + QString::number(serverTime - clientTime.toLongLong()) + "</p>\r\n";
        }

        emit BaseMessage::SendResponse(_guid, response);
    }
} // namespace Logic::MessageFactory