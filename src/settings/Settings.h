#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QDir>
#include <QSharedPointer>


//! @brief Класс для работы с настройками приложения
class Settings final
{
public:
    //! @brief Получить экземпляр класса
    //! @return Указатель на экземпляр класса
    static Settings* GetInstance()
    {
        static Settings instance;
        return &instance;
    }

    //! @brief Ручная синхронизация настроек
    void Sync();

    //! @brief Получить порт для прослушивания
    //! @return Порт для прослушивания
    //! @details По умолчанию 8080
    quint16 GetListenPort() const;

    //! @brief Установить имя базы данных
    //! @param name Имя базы данных
    //! @details Если имя не указано, используется значение по умолчанию "notification_server.db"
    QString GetDatabaseName() const;

    //! @brief Получить время истечения ключа подключения
    //! @return Время истечения ключа подключения в днях
    //! @details По умолчанию 30 дней
    int GetKeyExpirationTime() const;

    //! @brief Получить значение, включена ли криптография
    //! @return true, если криптография включена, иначе false
    //! @details По умолчанию true
    bool GetEnableCrypto() const;

private:
    //! @brief Конструктор по умолчанию
    Settings()
    {
        // Инициализация настроек с файлом конфигурации по умолчанию
        _settings = QSharedPointer<QSettings>::create("server.conf", QSettings::IniFormat);

        // Убедимся, что файл настроек существует
        if(!_settings->fileName().isEmpty())
        {
            _settings->beginGroup("ethernet");
            _settings->endGroup();
        }
    }

    //! @brief Деструктор по умолчанию
    ~Settings() = default;

    //! @brief Запрет копирования
    //! @param other Другой объект
    Settings(const Settings& other) = delete;

    //! @brief Запрет перемещения
    //! @param other Другой объект
    Settings(Settings&& other) = delete;

    //! @brief Запрет присваивания
    //! @param other Другой объект
    Settings& operator=(const Settings& other) = delete;

    //! @brief Указатель на настройки
    //! @details Используется для хранения настроек приложения
    QSharedPointer<QSettings> _settings;
};

#endif // SETTINGS_H