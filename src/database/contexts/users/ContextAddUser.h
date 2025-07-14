#ifndef CONTEXT_ADD_USER_H
#define CONTEXT_ADD_USER_H

#include "BaseContextUser.h"

namespace Database::Contexts
{
    //! @brief Базовый контекст пользователя
    class BaseContextUser;

    //! @brief Контекст для добавления нового пользователя
    class ContextAddUser final : public BaseContextUser
    {
    public:
        using BaseContextUser::username;
        using BaseContextUser::password;
        using BaseContextUser::email;

        //! @brief Конструктор по умолчанию
        ContextAddUser() = default;

        //! @brief Конструктор копирования
        ContextAddUser(const ContextAddUser& other) : BaseContextUser(other) {}

        //! @brief Конструктор перемещения
        ContextAddUser(ContextAddUser&& other) noexcept : BaseContextUser(std::move(other)) {}

        //! @brief Проверяет, заполнены ли обязательные поля
        //! @return true, если все обязательные поля заполнены, иначе false
        //! @details Обязательные поля: username, password, email
        bool ChechRequiredFields() const override
        {
            return !username.isEmpty() &&
                !password.isEmpty() &&
                !email.isEmpty();
        }
    };

}; // namespace Database::Contexts

#endif // CONTEXT_ADD_USER_H