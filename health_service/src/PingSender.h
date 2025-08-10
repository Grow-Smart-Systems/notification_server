#pragma once

#include <string>
#include <mutex>

#include "ServiceInfo.hpp"


class PingSender
{
public:
    /// @brief Отправить ping-запрос
    /// @param address Адрес сервиса
    void SendPing(ServiceInfo& address);

private:
    /// @brief Получить текущую дату и время
    /// @return Строка с текущей датой и временем
    std::string getCurrentDateTime();

    /// @brief Мьютекс для синхронизации доступа к информации о сервисе
    std::mutex _serviceInfoMutex;
};
