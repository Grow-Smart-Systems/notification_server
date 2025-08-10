#pragma once

#include <string>


/// @brief Структура для хранения информации о сервисе
struct ServiceInfo
{
    /// @brief Название сервиса
    std::string name;

    /// @brief Адрес сервиса
    std::string address;

    /// @brief Порт сервиса
    int port{ 5000 };

    /// @brief Статус сервиса
    bool status{ false };
};