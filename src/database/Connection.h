#ifndef CONNECTION_H
#define CONNECTION_H

#include "Defines.h"


namespace Database
{
    class Connection final : public QObject
    {
        Q_OBJECT

    public:
        //! @brief Конструктор по умолчанию
        //! @details Создает объект соединения с базой данных без инициализации
        Connection(const QString& dbPath);

        //! @brief Открывает соединение с базой данных для текущего потока
        //! @param connectionName Уникальное имя подключения для потока
        //! @return true, если соединение успешно открыто, иначе false
        bool Open(const QString& connectionName = QString());

        //! @brief Закрывает соединение с базой данных для текущего потока
        //! @param connectionName Имя подключения
        void Close(const QString& connectionName = QString());

        //! @brief Запускает SQL-запрос в рамках подключения с указанным именем
        //! @param queryString Строка запроса SQL
        //! @param connectionName Имя подключения
        //! @return Результат выполнения запроса, содержащий статус и список значений
        QueryResult RunQuery(const QString& queryString, const QString& connectionName = QString());

        //! @brief Запускает SQL-запрос в рамках подключения с указанным именем
        //! @param queryString Строка запроса SQL
        //! @param connectionName Имя подключения
        //! @return Результат выполнения запроса, содержащий статус и список значений
        QueryResult RunSingleQuery(const QString& queryString, const QString& connectionName = QString());

    private:
        //! @brief Получить подключение к базе данных по имени
        static QSqlDatabase getDatabase(const QString& connectionName);

        //! @brief Инициализирует схему базы данных для подключения
        //! @param db Ссылка на объект базы данных
        //! @return true, если схема успешно инициализирована, иначе false
        bool initSchema();

        //! @brief Путь к файлу базы данных
        QString _dbPath;
        //! @brief Конструктор по умолчанию
        Connection(const Connection&) = delete;

        //! @brief Оператор присваивания копированием
        Connection(Connection&&) = delete;

        
        Connection& operator=(const Connection&) = delete;
    };
}; // namespace Database
#endif // CONNECTION_H
