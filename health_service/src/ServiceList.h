#pragma once

#include <string>
#include <vector>

#include "ServiceInfo.hpp"


class ServiceList final
{
public:
    /// @brief Получить список адресов из переменной окружения SERVICE_ADDRESSES
    /// @note Путь до файла берется из переменной окружения SERVICES_JSON_PATH
    void UpdateServiceInfo();

    /// @brief Получить информацию о сервисах
    /// @return Список сервисов
    std::vector<ServiceInfo>& GetServiceInfo() noexcept;

    /// @brief Получить список сервисов из JSON-файла
    /// @param path Путь к JSON-файлу
    /// @return Список сервисов
    std::vector<ServiceInfo> GetServiceListFromJson(const std::string& path);

private:
    /// @brief Список сервисов
    std::vector<ServiceInfo> _services;

    /// @brief Путь по умолчанию к JSON-файлу с сервисами
    std::string _jsonPath;
};
