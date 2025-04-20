#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QDir>
#include <QSharedPointer>


/// @brief Класс для работы с настройками приложения
class Settings final
{
public:
    ///@brief Получить экземпляр класса
    ///@return Указатель на экземпляр класса
    static Settings* getInstance()
    {
        static Settings instance;
        return &instance;
    }

    ///@brief Ручная синхронизация настроек
    void sync();

    /// @brief Получить директорию конфигурации
    /// @return Директория конфигурации
    QDir getConfigDir() const;

    /// @brief Получить порт для прослушивания
    /// @return Порт для прослушивания
    /// @details По умолчанию 8080
    quint16 getListenPort() const;

private:
    ///@brief Конструктор по умолчанию
    Settings() = default;

    ///@brief Деструктор по умолчанию
    ~Settings() = default;

    ///@brief Запрет копирования
    ///@param other Другой объект
    Settings(const Settings& other) = delete;

    ///@brief Запрет перемещения
    ///@param other Другой объект
    Settings(Settings&& other) = delete;

    ///@brief Запрет присваивания
    ///@param other Другой объект
    Settings& operator=(const Settings& other) = delete;

    ///@brief Указатель на настройки
    ///@details Используется для хранения настроек приложения
    QSharedPointer<QSettings> _settings;
};

#endif // SETTINGS_H