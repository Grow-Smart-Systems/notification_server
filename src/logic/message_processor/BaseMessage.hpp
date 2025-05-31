#ifndef BASEMESSAGE_H
#define BASEMESSAGE_H

#include "Defines.h"

// Базовый класс для сообщений
class BaseMessage {
public:
    virtual ~BaseMessage() = default;
    virtual QString type() const = 0;
};

#endif // BASEMESSAGE_H