#ifndef CONTEXT_SET_KEY_FOR_DEVICE_H
#define CONTEXT_SET_KEY_FOR_DEVICE_H

#include "BaseContextConnectionKey.h"


namespace Database::Contexts
{
    //! @brief Контекст для привязки устройства к ключу подключения
    class ContextSetPublicKeyForDevice : public BaseContextConnectionKey
    {
    public:
        using BaseContextConnectionKey::clientPublicKey;
        using BaseContextConnectionKey::deviceID;

        //! @brief Конструктор по умолчанию
        ContextSetPublicKeyForDevice() = default;

        //! @brief Конструктор копирования
        ContextSetPublicKeyForDevice(const ContextSetPublicKeyForDevice& other) :
            BaseContextConnectionKey(other) {}

        //! @brief Конструктор перемещения
        ContextSetPublicKeyForDevice(ContextSetPublicKeyForDevice&& other) noexcept :
            BaseContextConnectionKey(std::move(other)) {}
        
        //! @brief Проверяет, заполнены ли обязательные поля
        bool CheckRequiredFields() const override
        {
            return !publicKey.isEmpty() &&
                   deviceID >= 0;
        }
    };
} // namespace Database::Contexts

#endif