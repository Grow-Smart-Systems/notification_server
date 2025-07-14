#ifndef CONTEXT_ADD_KEY_H
#define CONTEXT_ADD_KEY_H

#include "BaseContextConnectionKey.h"

namespace Database::Contexts
{
    //! @brief Контекст для добавления нового ключа подключения
    class ContextCreateKey final : public BaseContextConnectionKey
    {
    public:
        using BaseContextConnectionKey::publicKey;
        using BaseContextConnectionKey::privateKey;
        using BaseContextConnectionKey::deviceID;
        using BaseContextConnectionKey::userID;

        //! @brief Конструктор по умолчанию
        ContextCreateKey() = default;

        //! @brief Конструктор копирования
        ContextCreateKey(const ContextCreateKey& other) : BaseContextConnectionKey(other) {}

        //! @brief Конструктор перемещения
        ContextCreateKey(ContextCreateKey&& other) noexcept : BaseContextConnectionKey(std::move(other)) {}

        //! @brief Проверяет, заполнены ли обязательные поля
        bool CheckRequiredFields() const override
        {
            return !publicKey.isEmpty() &&
                   !privateKey.isEmpty() &&
                   deviceID >= 0 &&
                   userID >= 0;
        }
    };
} // namespace Database::Contexts

#endif // CONTEXT_ADD_KEY_H