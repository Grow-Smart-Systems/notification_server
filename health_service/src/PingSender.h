#pragma once

#include <string>

#include "ServiceInfo.hpp"


class PingSender
{
public:
    /// @brief Отправить ping-запрос
    /// @param address Адрес сервиса
    void SendPing(ServiceInfo& address);
};
