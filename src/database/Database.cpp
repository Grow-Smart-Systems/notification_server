#include "Database.h"

bool Database::Database::AddUser(const Contexts::ContextAddUser& context)
{
    // Проверяем, что контекст пользователя валиден
    if (!context.ChechRequiredFields())
    {
        qWarning("Invalid user context provided.");
        return false;
    }

    auto connectionName = openConnection();

    // Проверим, что такой пользователь уже не существует
    auto checkQuery = QString("SELECT COUNT(*) FROM users WHERE username = '%1'").arg(context.username);
    auto result = connection->RunQuery(checkQuery, connectionName);

    // Если запрос выполнен с ошибкой или пользователь с таким именем не найден
    if (!result.first || result.second.isEmpty())
    {
        qWarning("Failed to check if user exists: %s", qPrintable(connectionName));
        closeConnection(connectionName);
        throw ErrorCode::DatabaseError;
    }
    if (result.second[0].toInt() == 0)
    {
        qWarning("User with username '%s' already exists.", qPrintable(context.username));
        closeConnection(connectionName);
        throw ErrorCode::UserAlreadyExists;
    }

    auto currentDateTime = QDateTime::currentDateTime().toString(Qt::ISODate);
    auto query = QString("INSERT INTO users "
        "(username, password, email, created_at, last_seen_date) "
        "VALUES ('%1', '%2', '%3', '%4, '%5')")
        .arg(context.username,
            context.password,
            context.email,
            currentDateTime,
            currentDateTime);

    return runQueryAndCloseConnection(query, connectionName);
}

bool Database::Database::AddDevice(const Contexts::ContextAddDevice& context)
{
    if(!context.ChechRequiredFields())
    {
        qWarning("Invalid device context provided.");
        return false;
    }

    auto connectionName = openConnection();

    // Проверим, что устройство с таким GUID уже не существует
    auto checkQuery = QString("SELECT COUNT(*) FROM devices WHERE guid = '%1'").arg(context.guid);
    auto result = connection->RunQuery(checkQuery, connectionName);

    // Если запрос выполнен с ошибкой или устройство с таким GUID не найдено
    if (!result.first || result.second.isEmpty())
    {
        qWarning("Failed to check if device exists: %s", qPrintable(connectionName));
        closeConnection(connectionName);
        throw ErrorCode::DatabaseError;
    }

    // Если запрос выполнен успешно и устройство с таким GUID уже существует
    if (result.second[0].toInt() > 0)
    {
        qWarning("Device with GUID '%s' already exists.", qPrintable(context.guid));
        closeConnection(connectionName);
        throw ErrorCode::DeviceAlreadyExists;
    }

    auto currentDateTime = QDateTime::currentDateTime().toString(Qt::ISODate);
    auto query = QString("INSERT INTO devices "
        "(user_id, device_type, ip_address, registration_date, last_seen_date, guid) "
        "VALUES (%1, %2, '%3', '%4', '%5', '%6')")
        .arg(context.userID)
        .arg(static_cast<int>(context.deviceType))
        .arg(context.ipAddress)
        .arg(currentDateTime)
        .arg(currentDateTime)
        .arg(context.guid);

    return runQueryAndCloseConnection(query, connectionName);
}

bool Database::Database::CreateConnectionKey(const Contexts::ContextCreateKey& context)
{
    if (!context.CheckRequiredFields())
    {
        qWarning("Invalid connection key context provided.");
        return false;
    }

    auto connectionName = openConnection();

    // Если указано устройство, отметим старые ключи для этого устройства
    // и удалим их из базы данных
    if (context.deviceID != 0)
    {
        // Найдем другие ключи для этого устройства
        auto findKeysQuery = QString("DELETE FROM connection_keys WHERE device_id = %1")
            .arg(context.deviceID);
        auto result = connection->RunQuery(findKeysQuery, connectionName);

        // Если запрос выполнен с ошибкой или не найдено других ключей
        if (!result.first)
        {
            qWarning("Failed to find existing keys for device ID %d: %s", 
                    context.deviceID, 
                    qPrintable(connectionName));
            closeConnection(connectionName);
            throw ErrorCode::DatabaseError;
        }
    }
    
    // Создаем новый ключ подключения
    auto currentDateTime = QDateTime::currentDateTime().toString(Qt::ISODate);
    auto query = QString("INSERT INTO connection_keys "
        "(public_key, private_key, device_id, user_id, created_at, is_using) "
        "VALUES ('%1', '%2', %3, %4, '%5', %6)")
        .arg(QString(context.publicKey.toHex()))
        .arg(QString(context.privateKey.toHex()))
        .arg(context.deviceID)
        .arg(context.userID)
        .arg(currentDateTime)
        .arg(0);

    return runQueryAndCloseConnection(query, connectionName);
}

bool Database::Database::SetPublicKeyForDevice(const Contexts::ContextSetPublicKeyForDevice& context)
{
    if(!context.CheckRequiredFields())
    {
        qWarning("Invalid device ID context for key provided.");
        return false;
    }

    auto connectionName = openConnection();

    // Установим публичный ключ для устройства
    auto query = QString("UPDATE connection_keys SET client_public_key = %1 WHERE device_id = '%2'")
        .arg(QString(context.clientPublicKey.toHex()))
        .arg(context.deviceID);

    return runQueryAndCloseConnection(query, connectionName);
}

bool Database::Database::CheckModelVersion(const Contexts::ContextCheckModelVersion& context)
{
    if (!context.CheckRequiredFields())
    {
        qWarning("Invalid model version context provided.");
        return false;
    }

    auto connectionName = openConnection();

    // Проверим, что версия модели соответствует ожидаемой
    auto query = QString("SELECT COUNT(*) FROM model_versions WHERE version = %1").arg(context.version);
    auto result = connection->RunQuery(query, connectionName);
    closeConnection(connectionName);

    // Если запрос выполнен с ошибкой или версия модели не найдена
    if (!result.first || result.second.isEmpty())
    {
        qWarning("Failed to check model version: %s", qPrintable(connectionName));
        throw ErrorCode::DatabaseError;
    }

    // Если запрос выполнен успешно и версия модели не соответствует ожидаемой
    if (result.second[0].toInt() > 0)
    {
        qWarning("Model version %d matches the expected version.", context.version);
        return false;
    }
    return true;
}

bool Database::Database::GetPrivateKeyByClientKey(Contexts::ContextGetPrivateKeyByClientKey& context)
{
    if (!context.CheckRequiredFields())
    {
        qWarning("Invalid context for getting private key.");
        return false;
    }

    auto connectionName = openConnection();

    // Выполним запрос на получение приватного ключа и GUID устройства
    auto query = QString("SELECT ck.private_key, d.guid "
                         "FROM connection_keys ck "
                         "LEFT JOIN devices d ON ck.device_id = d.id "
                         "WHERE ck.public_key = '%1';")
        .arg(QString(context.publicKey.toHex()));
    auto result = connection->RunQuery(query, connectionName);
    closeConnection(connectionName);

    // Если запрос выполнен с ошибкой или приватный ключ не найден
    if (!result.first || result.second.isEmpty() || result.second.size() < 2)
    {
        qWarning("Failed to get private key: %s", qPrintable(connectionName));
        return false;
    }

    // Если запрос выполнен успешно, сохраняем приватный ключ в контексте
    context.privateKey = QByteArray::fromHex(result.second[0].toByteArray());
    context.GUID = result.second[1].toString();
    return true;
}

bool Database::Database::GetPublicClientKeyByGUID(Contexts::ContextGetPublicClientKeyByGUID& context)
{
    if (!context.CheckRequiredFields())
    {
        qWarning("Invalid context for getting public client key by GUID.");
        return false;
    }

    auto connectionName = openConnection();

    // Выполним запрос на получение публичного ключа клиента по GUID устройства
    auto query = QString("SELECT ck.client_public_key "
                         "FROM connection_keys ck "
                         "LEFT JOIN devices d ON ck.device_id = d.id "
                         "WHERE d.guid = '%1';")
        .arg(context.deviceGuid.toQString());
    auto result = connection->RunQuery(query, connectionName);
    closeConnection(connectionName);

    // Если запрос выполнен с ошибкой или публичный ключ не найден
    if (!result.first || result.second.isEmpty())
    {
        qWarning("Failed to get public client key by GUID: %s", qPrintable(connectionName));
        return false;
    }

    // Если запрос выполнен успешно, сохраняем публичный ключ в контексте
    context.clientPublicKey = QByteArray::fromHex(result.second[0].toByteArray());
    return true;
}

QString Database::Database::openConnection()
{
    QString connectionName = QString::number(getCurrentThreadID());
    if (!connection->Open(connectionName))
    {
        qFatal("Failed to open database connection for thread %s.", qPrintable(connectionName));
    }
    return connectionName;
}

void Database::Database::closeConnection(const QString& connectionName)
{
    connection->Close(connectionName);
}

bool Database::Database::runQueryAndCloseConnection(const QString& query, const QString& connectionName)
{
    auto result = connection->RunQuery(query, connectionName);
    closeConnection(connectionName);
    return result.first;
}

Database::Database::Database()
{
    connection = std::make_unique<Connection>(
            Settings::GetInstance()->GetDatabaseName());
    if (!connection)
        qFatal("Failed to create database connection.");
    qInfo() << "Database connection created successfully.";
}

qint64 Database::Database::getCurrentThreadID() const
{
    return (quintptr)QThread::currentThreadId();
}
