#ifndef BASE_CONTEXT_DEVICE_H
#define BASE_CONTEXT_DEVICE_H

#include "QString"
#include <QDateTime>

namespace Database::Contexts
{
    //! @brief Контекст устройства
    class BaseContextDevice
    {
    public:
        //! @brief Тип устройства
        //! @details Используется для идентификации типа устройства в базе данных
        enum class DeviceType
        {
            //! @brief Неизвестный тип устройства
            Unknown = 0,

            //! @brief Дымовой датчик (1)
            SmokeDetectorV1 = 1,

            //! @brief Горшок для растений (v1)
            PlantPotV1 = 2
        };

    protected:
        //! @brief Уникальный идентификатор устройства
        int deviceID{ 0 };

        //! @brief ID пользователя, которому принадлежит устройство
        int userID{ 0 };

        //! @brief Тип устройства
        DeviceType deviceType{ 0 };

        //! @brief IP-адрес устройства
        QString ipAddress;

        //! @brief Дата регистрации устройства
        QDateTime registrationDate;

        //! @brief Дата последнего появления устройства
        QDateTime lastSeenDate;

        //! @brief Guid устройства
        QString guid;

        //! @brief Проверяет, заполнены ли обязательные поля
        //! @return true, если все обязательные поля заполнены, иначе false
        virtual bool ChechRequiredFields() const = 0;

        //! @brief Конструктор по умолчанию
        BaseContextDevice() = default;

        //! @brief Конструктор копирования
        BaseContextDevice(const BaseContextDevice& other)
            : deviceID(other.deviceID), userID(other.userID), deviceType(other.deviceType),
              ipAddress(other.ipAddress), registrationDate(other.registrationDate),
              lastSeenDate(other.lastSeenDate), guid(other.guid) {}

        //! @brief Конструктор перемещения
        BaseContextDevice(BaseContextDevice&& other) noexcept
            : deviceID(other.deviceID), userID(other.userID), deviceType(other.deviceType),
              ipAddress(std::move(other.ipAddress)), registrationDate(std::move(other.registrationDate)),
              lastSeenDate(std::move(other.lastSeenDate)), guid(std::move(other.guid)) {}

        //! @brief Оператор присваивания копированием
        BaseContextDevice& operator=(const BaseContextDevice& other)
        {
            if (this != &other)
            {
                deviceID = other.deviceID;
                userID = other.userID;
                deviceType = other.deviceType;
                ipAddress = other.ipAddress;
                registrationDate = other.registrationDate;
                lastSeenDate = other.lastSeenDate;
                guid = other.guid;
            }
            return *this;
        }

        //! @brief Оператор присваивания перемещением
        BaseContextDevice& operator=(BaseContextDevice&& other) noexcept
        {
            if (this != &other)
            {
                deviceID = other.deviceID;
                userID = other.userID;
                deviceType = other.deviceType;
                ipAddress = std::move(other.ipAddress);
                registrationDate = other.registrationDate;
                lastSeenDate = std::move(other.lastSeenDate);
                guid = std::move(other.guid);
            }
            return *this;
        }
    };
} // namespace Database::Contexts


#endif