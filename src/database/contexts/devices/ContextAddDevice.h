#ifndef CONTEXT_ADD_DEVICE_H
#define CONTEXT_ADD_DEVICE_H

#include "BaseContextDevice.h"

namespace Database::Contexts
{
    //! @brief Контекст устройства для добавления нового устройства
    class ContextAddDevice : public BaseContextDevice
    {
    public:
        using BaseContextDevice::userID;
        using BaseContextDevice::deviceType;
        using BaseContextDevice::ipAddress;
        using BaseContextDevice::guid;

        //! @brief Конструктор по умолчанию
        ContextAddDevice() = default;

        //! @brief Конструктор копирования
        ContextAddDevice(const ContextAddDevice& other) : BaseContextDevice(other) {}

        //! @brief Конструктор перемещения
        ContextAddDevice(ContextAddDevice&& other) noexcept : BaseContextDevice(std::move(other)) {}

        //! @brief Проверяет, заполнены ли обязательные поля
        bool ChechRequiredFields() const override
        {
            return !ipAddress.isEmpty() && !guid.isEmpty() && userID >= 0;
        }
    };

} // namespace Database::Contexts

#endif // CONTEXT_ADD_DEVICE_H