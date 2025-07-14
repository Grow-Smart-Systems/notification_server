#ifndef BASE_CONTEXT_CONNECTION_KEY_H
#define BASE_CONTEXT_CONNECTION_KEY_H

#include <QString>
#include <QDateTime>

namespace Database::Contexts
{
    //! @brief Контекст ключа подключения
    class BaseContextConnectionKey
    {
    protected:
        //! @brief Уникальный идентификатор ключа
        int id{ 0 };

        //! @brief Публичный ключ подключения
        QByteArray publicKey;

        //! @brief Публичный ключ клиента, который будет использоваться для шифрования данных
        QByteArray clientPublicKey;
        
        //! @brief Приватный ключ подключения
        QByteArray privateKey;

        //! @brief ID устройства, к которому привязан ключ
        int deviceID{ 0 };

        //! @brief ID пользователя, которому принадлежит ключ
        int userID{ 0 };

        //! @brief Дата создания ключа (ISO 8601)
        QDateTime createdAt;

        //! @brief Признак использования ключа (true — актуальный, false - неиспользуемый)
        bool isUsing{ false };

        //! @brief Проверяет, заполнены ли обязательные поля
        //! @return true, если все обязательные поля заполнены, иначе false
        virtual bool CheckRequiredFields() const = 0;

        //! @brief Конструктор по умолчанию
        BaseContextConnectionKey() = default;

        //! @brief Конструктор копирования
        BaseContextConnectionKey(const BaseContextConnectionKey& other)
            : id(other.id),
              publicKey(other.publicKey),
              privateKey(other.privateKey),
              deviceID(other.deviceID),
              userID(other.userID),
              createdAt(other.createdAt),
              isUsing(other.isUsing) {}

        //! @brief Конструктор перемещения
        BaseContextConnectionKey(BaseContextConnectionKey&& other) noexcept
            : id(other.id),
              publicKey(std::move(other.publicKey)),
              privateKey(std::move(other.privateKey)),
              deviceID(other.deviceID),
              userID(other.userID),
              createdAt(std::move(other.createdAt)),
              isUsing(other.isUsing) {}

        //! @brief Оператор присваивания копированием
        BaseContextConnectionKey& operator=(const BaseContextConnectionKey& other)
        {
            if (this != &other)
            {
                id = other.id;
                publicKey = other.publicKey;
                privateKey = other.privateKey;
                deviceID = other.deviceID;
                userID = other.userID;
                createdAt = other.createdAt;
                isUsing = other.isUsing;
            }
            return *this;
        }

        //! @brief Оператор присваивания перемещением
        BaseContextConnectionKey& operator=(BaseContextConnectionKey&& other) noexcept
        {
            if (this != &other)
            {
                id = other.id;
                publicKey = std::move(other.publicKey);
                privateKey = std::move(other.privateKey);
                deviceID = other.deviceID;
                userID = other.userID;
                createdAt = std::move(other.createdAt);
                isUsing = other.isUsing;
            }
            return *this;
        }
    };
} // namespace Database::Contexts

#endif
