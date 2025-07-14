#ifndef BASE_CONTEXT_GUIDS_H
#define BASE_CONTEXT_GUIDS_H

#include <QString>
#include <QDateTime>

namespace Database::Contexts
{
    //! @brief Контекст для работы с таблицей guids
    class ContextGuids
    {
    protected:
        //! @brief Уникальный идентификатор записи
        int id{0};
        
        //! @brief GUID устройства (уникальный)
        QString guid;

        //! @brief Дата регистрации GUID (ISO 8601)
        QDateTime registrationDate;

        //! @brief Конструктор по умолчанию
        ContextGuids() = default;

        //! @brief Конструктор копирования
        ContextGuids(const ContextGuids& other)
            : id(other.id), guid(other.guid), registrationDate(other.registrationDate) {}

        //! @brief Конструктор перемещения
        ContextGuids(ContextGuids&& other) noexcept
            : id(other.id), guid(std::move(other.guid)), registrationDate(std::move(other.registrationDate)) {}

        //! @brief Оператор присваивания копированием
        ContextGuids& operator=(const ContextGuids& other)
        {
            if (this != &other)
            {
                id = other.id;
                guid = other.guid;
                registrationDate = other.registrationDate;
            }
            return *this;
        }

        //! @brief Оператор присваивания перемещением
        ContextGuids& operator=(ContextGuids&& other) noexcept
        {
            if (this != &other)
            {
                id = other.id;
                guid = std::move(other.guid);
                registrationDate = std::move(other.registrationDate);
            }
            return *this;
        }

        //! @brief Проверяет, заполнены ли обязательные поля
        //! @return true, если все обязательные поля заполнены, иначе false
        virtual bool CheckRequiredFields() const = 0;
    };
} // namespace Database::Contexts

#endif