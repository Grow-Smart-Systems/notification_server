#ifndef BASE_CONTEXT_USER_H
#define BASE_CONTEXT_USER_H

#include <QString>
#include <QDateTime>

namespace Database::Contexts
{
    //! @brief Базовый контекст пользователя
    class BaseContextUser
    {
    protected:
        //! @brief Уникальный идентификатор пользователя
        int id{ 0 };

        //! @brief Имя пользователя (уникальное)
        QString username;

        //! @brief Пароль пользователя (хэш)
        QString password;

        //! @brief Email пользователя
        QString email;

        //! @brief Дата создания пользователя (ISO 8601)
        QDateTime createdAt;

        //! @brief Дата последнего входа пользователя (ISO 8601)
        QDateTime lastSeenDate;

        //! @brief Проверяет, заполнены ли обязательные поля
        //! @return true, если все обязательные поля заполнены, иначе false
        virtual bool ChechRequiredFields() const = 0;

        //! @brief Конструктор по умолчанию
        BaseContextUser() = default;

        //! @brief Конструктор копирования
        BaseContextUser(const BaseContextUser& other) 
            : id(other.id), username(other.username), password(other.password),
              email(other.email), createdAt(other.createdAt), lastSeenDate(other.lastSeenDate) 
        {
        }

        //! @brief Конструктор перемещения
        BaseContextUser(BaseContextUser&& other) noexcept :
            id(other.id),
            username(std::move(other.username)),
            password(std::move(other.password)),
            email(std::move(other.email)),
            createdAt(std::move(other.createdAt)),
            lastSeenDate(std::move(other.lastSeenDate))
        {
        }

        //! @brief Оператор присваивания копированием
        BaseContextUser& operator=(const BaseContextUser& other) 
        {
            if (this != &other) 
            {
                id = other.id;
                username = other.username;
                password = other.password;
                email = other.email;
                createdAt = other.createdAt;
                lastSeenDate = other.lastSeenDate;
            }
            return *this;
        }
    };
} // namespace Database::Contexts

#endif // BASE_CONTEXT_USER_H
