#ifndef CONTEXT_GET_PRIVATE_KEY_H
#define CONTEXT_GET_PRIVATE_KEY_H

#include "BaseContextConnectionKey.h"


namespace Database::Contexts
{
    //! @brief Контекст для получения приватного ключа по публичному ключу клиента
    class ContextGetPrivateKeyByClientKey : public BaseContextConnectionKey
    {
    public:
        // Входные параметры
        using BaseContextConnectionKey::publicKey;

        // Выходные параметры
        using BaseContextConnectionKey::privateKey;

        //! @brief GUID устройства, к которому привязан ключ
        QString GUID;


        //! @brief Конструктор по умолчанию
        ContextGetPrivateKeyByClientKey() = default;

        //! @brief Конструктор копирования
        ContextGetPrivateKeyByClientKey(const ContextGetPrivateKeyByClientKey& other) :
            BaseContextConnectionKey(other) {}

        //! @brief Конструктор перемещения
        ContextGetPrivateKeyByClientKey(ContextGetPrivateKeyByClientKey&& other) noexcept :
            BaseContextConnectionKey(std::move(other)) {}
        
        //! @brief Проверяет, заполнены ли обязательные поля
        bool CheckRequiredFields() const override
        {
            return !publicKey.isEmpty();
        }
    };
} // namespace Database::Contexts

#endif