#ifndef GUID_H
#define GUID_H

#include <string>
#include <string_view>
#include <regex>
#include <utility>
#include <functional>

//! @brief Класс для работы с GUID
//! @details Представляет собой уникальный идентификатор в формате "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
//! @note нулевой GUID имеет значение "00000000-0000-0000-0000-000000000000" и считается невалидным
class GUID final
{
    //! @brief Нулевой GUID
    static constexpr const char* NULL_GUID = "00000000-0000-0000-0000-000000000000";

public:
    //! @brief Конструктор по умолчанию
    constexpr GUID() noexcept = default;

    //! @brief Конструктор с параметром const char*
    //! @param guid Строка GUID в формате const char*
    GUID(const char* guid) : _guid(guid ? guid : NULL_GUID)
    {
        normalize();
    }

    //! @brief Конструктор с параметром
    //! @param guid Строка GUID
    explicit GUID(std::string_view guid) : _guid(guid)
    {
        normalize();
    }

    //! @brief Конструктор с параметром std::string
    //! @param guid Строка GUID
    explicit GUID(const std::string& guid) : _guid(guid)
    {
        normalize();
    }

    //! @brief Конструктор с параметром std::string (move)
    //! @param guid Строка GUID
    explicit GUID(std::string&& guid) noexcept : _guid(std::move(guid))
    {
        normalize();
    }

    //! @brief Конструктор копирования
    //! @param other Другой GUID
    GUID(const GUID& other) = default;

    //! @brief Конструктор перемещения
    //! @param other Другой GUID
    GUID(GUID&& other) noexcept = default;

    //! @brief Оператор присваивания
    //! @param other Другой GUID
    //! @return Ссылка на текущий объект
    GUID& operator=(const GUID& other) = default;

    //! @brief Оператор присваивания (move)
    //! @param other Другой GUID
    //! @return Ссылка на текущий объект
    GUID& operator=(GUID&& other) noexcept = default;

    //! @brief Оператор сравнения на равенство
    //! @param other Другой GUID
    //! @return true, если GUID равны, иначе false
    bool operator==(const GUID& other) const noexcept { return _guid == other._guid; }

    //! @brief Оператор сравнения на неравенство
    //! @param other Другой GUID
    //! @return true, если GUID не равны, иначе false
    bool operator!=(const GUID& other) const noexcept { return !(*this == other); }

    //! @brief Оператор "меньше" для сортировки
    //! @param other Другой GUID
    //! @return true, если текущий GUID меньше other
    bool operator<(const GUID& other) const noexcept { return _guid < other._guid; }

    //! @brief Оператор "больше" для сортировки
    //! @param other Другой GUID
    //! @return true, если текущий GUID больше other
    bool operator>(const GUID& other) const noexcept { return other < *this; }

    //! @brief Оператор "меньше или равно" для сортировки
    //! @param other Другой GUID
    //! @return true, если текущий GUID меньше или равен other
    bool operator<=(const GUID& other) const noexcept { return !(*this > other); }

    //! @brief Оператор "больше или равно" для сортировки
    //! @param other Другой GUID
    //! @return true, если текущий GUID больше или равен other
    bool operator>=(const GUID& other) const noexcept { return !(*this < other); }

    //! @brief Возвращает GUID в виде строки
    //! @return Строка GUID
    const std::string& toString() const noexcept { return _guid; }

    //! @brief Устанавливает GUID нулевым
    //! @details Устанавливает GUID в значение "00000000-0000-0000-0000-000000000000"
    void SetNull() noexcept
    {
        _guid = NULL_GUID;
    }

    //! @brief Устанавливает GUID
    //! @param guid Строка GUID
    void SetGuid(std::string_view guid)
    {
        _guid = guid;
        normalize();
    }

    //! @brief Проверяет, является ли GUID валидным
    //! @return true, если GUID валиден, иначе false
    bool IsValid() const noexcept
    {
        return isValidFormat(_guid) && !IsNull();
    }

    //! @brief Проверяет, является ли GUID нулевым
    //! @return true, если GUID нулевой, иначе false
    bool IsNull() const noexcept
    {
        return _guid == NULL_GUID;
    }

    //! @brief Возвращает хэш GUID для использования в контейнерах
    //! @return Хэш строки GUID
    std::size_t hash() const noexcept
    {
        return std::hash<std::string>{}(_guid);
    }

private:
    //! @brief Нормализует GUID (устанавливает NULL_GUID для пустых/некорректных значений)
    void normalize() noexcept
    {
        if (_guid.empty() || _guid == NULL_GUID) {
            _guid = NULL_GUID;
        }
    }

    //! @brief Проверяет формат GUID
    //! @param guid Строка для проверки
    //! @return true, если формат корректен
    static bool isValidFormat(std::string_view guid) noexcept
    {
        static const std::regex re(
            "^[0-9a-fA-F]{8}-"
            "[0-9a-fA-F]{4}-"
            "[0-9a-fA-F]{4}-"
            "[0-9a-fA-F]{4}-"
            "[0-9a-fA-F]{12}$"
        );
        return std::regex_match(guid.begin(), guid.end(), re);
    }
    //! @brief GUID в виде строки
    //! @details Хранит GUID в формате "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
    //! @note Изначально установлен в значение NULL_GUID
    std::string _guid{ NULL_GUID };
};

#endif // GUID_H