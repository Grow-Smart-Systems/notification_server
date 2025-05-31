#ifndef MESSEGEFACTORY_H
#define MESSEGEFACTORY_H

#include "Defines.h"
#include "BaseMessage.h"

class MessageFactory
{
public:
    // Пример статического метода для разбора сообщения
    // Можно возвращать std::unique_ptr<BaseMessage> или QVariant для универсальности
    static std::unique_ptr<BaseMessage> parseMessage(const QString& rawMessage);
};

#endif // MESSEGEFACTORY_H