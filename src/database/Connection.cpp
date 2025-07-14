#include "Connection.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

using namespace Database;

Database::Connection::Connection(const QString& dbPath) :
    _dbPath(dbPath)
{
    // Инициализация схемы базы данных
    if (!initSchema())
    {
        qFatal("Failed to initialize database schema.");
    }
}

bool Database::Connection::Open(const QString& connectionName)
{
    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName))
    {
        db = QSqlDatabase::database(connectionName);
        if (db.isOpen())
        {
            qWarning() << "Database is already open for connection:" << connectionName;
            return true;
        }
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_dbPath);
    }
    if (!db.open())
    {
        qCritical() << "Failed to open database for connection" << connectionName << ":" << db.lastError().text();
        return false;
    }
    return true;
}

void Connection::Close(const QString& connectionName)
{
    if (!QSqlDatabase::contains(connectionName)) 
        return;

    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (db.isOpen())
    {
        db.close();
    }
    QSqlDatabase::removeDatabase(connectionName);
}

QueryResult Connection::RunQuery(const QString& queryString, const QString& connectionName)
{
    if (queryString.isEmpty())
    {
        qWarning() << "Query string is empty.";
        return {false, {}};
    }

    auto db = getDatabase(connectionName);
    if (!db.isOpen())
    {
        qCritical() << "Database is not open for connection:" << connectionName;
        return {false, {}};
    }

    QSqlQuery query(db);
    query.exec(queryString);
    if (query.lastError().isValid())
    {
        qCritical() << "Query error:" << query.lastError().text();
        return {false, {}};
    }

    QList<QVariant> results;
    while (query.next())
    {
        QList<QVariant> row;
        for (int i = 0; i < query.record().count(); ++i)
        {
            row.append(query.value(i));
        }
        results.append(row);
    }
    return {true, results};
}

QSqlDatabase Connection::getDatabase(const QString& connectionName)
{
    if (!QSqlDatabase::contains(connectionName))
    {
        qCritical() << "No database connection with name:" << connectionName;
    }
    return QSqlDatabase::database(connectionName);
}

QueryResult Connection::RunSingleQuery(const QString& queryString, const QString& connectionName)
{
    if(!Open(connectionName))
    {
        qCritical() << "Failed to open database connection for query execution:" << connectionName;
        return {false, {}};
    }
    QueryResult result = RunQuery(queryString, connectionName);
    Close(connectionName);
    return result;
}

bool Connection::initSchema()
{
    Open();
    if(!RunQuery("CREATE TABLE IF NOT EXISTS device_types (" // Таблица типов устройств
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"     // ID типа устройства
                 "short_name TEXT NOT NULL,"                 // Краткое название типа
                 "full_name TEXT NOT NULL"                   // Полное название типа
                 ")").first)
        return false;

    // Таблица устройств
    if(!RunQuery("CREATE TABLE IF NOT EXISTS devices ("     // Таблица устройств
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"    // ID устройства
                 "user_id INTEGER NOT NULL,"                // ID пользователя, которому принадлежит устройство
                 "device_type INTEGER NOT NULL,"            // Тип устройства
                 "ip_address TEXT NOT NULL,"                // IP-адрес устройства
                 "registration_date TEXT NOT NULL,"         // Дата регистрации устройства
                 "last_seen_date TEXT NOT NULL,"            // Дата последнего посещения
                 "guid TEXT NOT NULL,"                      // Guid устройства
                 "FOREIGN KEY(user_id) REFERENCES users(id)"// Внешний ключ на пользователя
                 "FOREIGN KEY(device_type) REFERENCES device_types(id)" // Внешний ключ на тип устройства
                 ")").first)
        return false;

    // Таблица ключей подключения
    if(!RunQuery("CREATE TABLE IF NOT EXISTS connection_keys (" // Таблица ключей подключения
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"        // ID ключа
                 "private_key TEXT NOT NULL,"                   // Приватный ключ
                 "public_key TEXT NOT NULL,"                    // Публичный ключ сервера
                 "client_public_key TEXT NOT NULL,"             // Публичный ключ клиента
                 "device_id INTEGER NOT NULL,"                  // ID устройства
                 "user_id INTEGER NOT NULL,"                    // ID пользователя
                 "created_at TEXT NOT NULL,"                    // Дата создания ключа
                 "is_using INTEGER NOT NULL DEFAULT 0,"         // Признак использования ключа
                 "FOREIGN KEY(device_id) REFERENCES devices(id)," // Внешний ключ на устройство
                 "FOREIGN KEY(user_id) REFERENCES users(id)"    // Внешний ключ на пользователя
                 ")").first)
        return false;

    // Таблица пользователей
    if(!RunQuery("CREATE TABLE IF NOT EXISTS users ("        // Таблица пользователей
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"     // ID пользователя
                 "username TEXT NOT NULL UNIQUE,"            // Имя пользователя
                 "password TEXT NOT NULL,"                   // Пароль (хэш)
                 "email TEXT,"                               // Email
                 "created_at TEXT NOT NULL"                  // Дата создания
                 ")").first)
        return false;

    // Таблица GUID устройств
    if(!RunQuery("CREATE TABLE IF NOT EXISTS guids ("        // Таблица GUID устройств
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"     // ID записи
                 "guid TEXT NOT NULL UNIQUE,"                // GUID устройства
                 "registration_date TEXT NOT NULL"           // Дата регистрации GUID
                 ")").first)
        return false;

    // Таблица единиц изменений и типов данных
    if(!RunQuery("CREATE TABLE IF NOT EXISTS change_units (" // Таблица единиц изменений
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"     // ID единицы
                 "name TEXT NOT NULL,"                       // Наименование
                 "short_name TEXT NOT NULL,"                 // Сокращённое название единицы
                 "full_name TEXT NOT NULL"                   // Полное название единицы
                 ")").first)
        return false;

    // Таблица типов данных, поступающих от устройств
    if(!RunQuery("CREATE TABLE IF NOT EXISTS data_types ("   // Таблица типов данных
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"     // ID типа данных
                 "name TEXT NOT NULL,"                       // Наименование поля
                 "field_key TEXT NOT NULL,"                  // Ключ-название поля
                 "change_unit_id INTEGER NOT NULL,"          // ID единицы изменения
                 "FOREIGN KEY(change_unit_id) REFERENCES change_units(id)" // Внешний ключ на единицу изменения
                 ")").first)
        return false;

    // Таблица версия модели
    if (!RunQuery("CREATE TABLE IF NOT EXISTS model_version ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"   // ID записи
                  "version INTEGER NOT NULL,"               // Номер версии схемы
                  "updated_at TEXT NOT NULL"                // Дата обновления (ISO 8601)
                  ")").first)
        return false;
    Close();
    return true;
}


/*
TODO переписать на использование ассимитиричных ключей.

Порядок:

- в таблице должна храниться пара ключей: публичный устройства и приватный сервера
- при добавлении нового устройства генерируется пара ключей сервера и оба записываются в базу данных
- публичный ключ передается на устройство посредсвом ввода пользователем
- приватный ключ хранится в базе данных
- при подключении устройства к серверу оно отправляет свой публичный ключ с использованием публичного ключа сервера
- сервер проверяет, что публичный ключ устройства соответствует приватному ключу сервера
- если ключи совпадают, сервер отправляет ответ с подтверждением подключения и записывает публичный ключ устройства 
в базу данных в замен существующего

Дополнительно:

- при отправке ответа сервер проверяет все ключи не помеченные как использованные
- ключ, не помеченный как использованный, живет в базе не более 1 часа

*/