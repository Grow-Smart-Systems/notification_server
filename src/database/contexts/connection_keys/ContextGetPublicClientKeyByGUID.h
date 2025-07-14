// filepath: /home/hospitaler17/projects/notification_server/src/database/contexts/connection_keys/ContextGetPublicKeyByGUID.h
#ifndef CONTEXT_GET_PUBLIC_CLIENT_KEY_BY_GUID_H
#define CONTEXT_GET_PUBLIC_CLIENT_KEY_BY_GUID_H

#include "BaseContextConnectionKey.h"
#include "GUID.h"

namespace Database::Contexts
{
    //! @brief Контекст для получения публичного ключа по GUID устройства
    class ContextGetPublicClientKeyByGUID : public BaseContextConnectionKey
    {
    public:
        using BaseContextConnectionKey::clientPublicKey;

        GUID deviceGuid;

        //! @brief Конструктор по умолчанию
        ContextGetPublicClientKeyByGUID() = default;

        //! @brief Конструктор копирования
        ContextGetPublicClientKeyByGUID(const ContextGetPublicClientKeyByGUID& other) :
            BaseContextConnectionKey(other) {}

        //! @brief Конструктор перемещения
        ContextGetPublicClientKeyByGUID(ContextGetPublicClientKeyByGUID&& other) noexcept :
            BaseContextConnectionKey(std::move(other)) {}

        //! @brief Проверяет, заполнены ли обязательные поля
        bool CheckRequiredFields() const override
        {
            return !deviceGuid.IsValid();
        }
    };
} // namespace Database::Contexts

#endif