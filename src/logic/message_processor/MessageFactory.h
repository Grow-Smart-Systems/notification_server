#ifndef MESSEGEFACTORY_H
#define MESSEGEFACTORY_H

#include "Defines.h"
#include "BaseMessage.h"

namespace Logic::MessageFactory
{
    //! @brief Фабрика для парсинга и создания объектов сообщений
    class MessageFactory
    {
    public:
        //! @brief Парсит строку и создает объект BaseMessage
        //! @param key Ключ для ответного сообщения
        //! @param packet HTTP пакет для обработки данных
        static std::unique_ptr<BaseMessage> parseMessage(
            const Ethernet::TCPSocketKey& key, 
            const Ethernet::HTTPPacket& packet);
    };
};

#endif // MESSEGEFACTORY_H