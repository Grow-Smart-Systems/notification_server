#ifndef MESSEGEFACTORY_H
#define MESSEGEFACTORY_H

#include "Defines.h"
#include "BaseMessage.h"

namespace Logic::MessageFactory
{
    //! @brief Фабрика для парсинга сообщений
    class MessageFactory
    {
    public:
        //! @brief Парсит пакет и создает объект BaseMessage
        //! @param key Ключ для ответного сообщения
        //! @param packet HTTP пакет для обработки данных
        static std::shared_ptr<BaseMessage> parseMessage(
            const GUID& deviceGuid, 
            const Ethernet::HTTPPacket& packet);
    };
};

#endif // MESSEGEFACTORY_H