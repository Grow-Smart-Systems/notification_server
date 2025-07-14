#ifndef DATABASE_H
#define DATABASE_H

#include "Defines.h"
#include "Connection.h"
#include "Contexts.h"

namespace Database
{
    //! @brief Коды ошибок для операций с базой данных
    enum class ErrorCode
    {
        //! @brief Неизвестная ошибка
        UnknownError = 0,

        //! @brief Пользователь уже существует
        UserAlreadyExists,

        //! @brief Устройство уже добавлено
        DeviceAlreadyExists,

        //! @brief Ключ подключения уже существует
        KeyAlreadyExists,

        //! @brief Версия модели базы данных не соответствует ожидаемой
        ModelVersionMismatch,

        //! @brief Ошибка базы данных
        DatabaseError
    };

    //! @brief Интерфейсный класс работы с базой данных
    class Database final : public QObject
    {
        Q_OBJECT

    public:
        //! @brief Получить глобальный экземпляр Database
        static Database& GetInstance()
        {
            static Database instance;
            return instance;
        }

        //! @brief Добавляет пользователя в базу данных
        //! @param context Контекст пользователя
        //! @return true, если пользователь успешно добавлен, иначе false
        bool AddUser(const Contexts::ContextAddUser& context);

        //! @brief Добавляет устройство в базу данных
        //! @param context Контекст устройства
        //! @return true, если устройство успешно добавлено, иначе false
        bool AddDevice(const Contexts::ContextAddDevice& context);

        //! @brief Добавляет ключ подключения в базу данных
        //! @param context Контекст ключа подключения
        //! @return true, если ключ успешно добавлен, иначе false
        bool CreateConnectionKey(const Contexts::ContextCreateKey& context);

        //! @brief Устанавливает публичный ключ для устройства
        //! @param context Контекст установки публичного ключа для устройства
        //! @return true, если публичный ключ успешно установлен для устройства, иначе false
        bool SetPublicKeyForDevice(const Contexts::ContextSetPublicKeyForDevice& context);

        //! @brief Проверяет версию модели базы данных
        //! @param context Контекст проверки версии модели
        //! @return true, если версия модели соответствует ожидаемой, иначе false
        bool CheckModelVersion(const Contexts::ContextCheckModelVersion& context);

        //! @brief Получает приватный ключ по публичному ключу клиента
        //! @param context Контекст получения приватного ключа
        //! @return true, если приватный ключ успешно получен, иначе false
        bool GetPrivateKeyByClientKey(Contexts::ContextGetPrivateKeyByClientKey& context);

        //! @brief Получает публичный ключ клиента по GUID устройства
        //! @param context Контекст получения публичного ключа клиента
        //! @return true, если публичный ключ успешно получен, иначе false
        bool GetPublicClientKeyByGUID(Contexts::ContextGetPublicClientKeyByGUID& context);

    private:
        //! @brief Открывает соединение с базой данных для текущего потока
        //! @details Если соединение уже открыто, ничего не делает
        //! @return Имя подключения, если успешно открыто, иначе пустая строка
        QString openConnection();

        //! @brief Закрывает соединение с базой данных для текущего потока
        //! @param connectionName Имя подключения
        void closeConnection(const QString& connectionName);

        //! @brief Выполняет SQL-запрос в рамках подключения с указанным именем
        //! @param query SQL-запрос в виде строки
        //! @param connectionName Имя подключения
        //! @return true, если запрос выполнен успешно, иначе false
        //! @details Закрывает соединение после выполнения запроса
        bool runQueryAndCloseConnection(const QString& query, const QString& connectionName);

        //! @brief Конструктор
        Database();

        //! @brief Деструктор
        ~Database() = default;

        //! @brief Запрет копирования
        Database(const Database&) = delete;

        //! @brief Запрет перемещения
        Database(Database&&) = delete;

        //! @brief Запрет присваивания
        Database& operator=(const Database&) = delete;

        //! @brief Получает ID текущего потока
        //! @return ID текущего потока
        qint64 getCurrentThreadID() const;

        //! @brief Указатель на соединение с базой данных
        std::unique_ptr<Connection> connection {nullptr};
    };
}; // namespace Database

#endif // DATABASE_H