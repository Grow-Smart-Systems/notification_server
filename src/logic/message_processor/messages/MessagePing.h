#ifndef MESSAGEPING_H
#define MESSAGEPING_H

#include "Defines.h"
#include "BaseMessage.h"

namespace Logic::MessageFactory
{
    //! \brief Класс MessagePing обрабатывает HTTP-запросы типа "ping".
    class MessagePing final : public BaseMessage
    {
    public:
        //! \brief Конструктор класса MessagePing.
        MessagePing(const Ethernet::TCPSocketKey& key, const Ethernet::HTTPPacket& packet)
            : BaseMessage(key, packet) {}

        //! \brief Деструктор класса MessagePing.
        ~MessagePing() = default;

    protected:
        //! \brief Валидация и парсинг входных данных
        void FormResponse() override;
    };
};

#endif // MESSAGEPING_H