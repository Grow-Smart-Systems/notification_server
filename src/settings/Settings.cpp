#include "Settings.h"

void Settings::Sync()
{
    if(_settings)
        _settings->sync();
}

quint16 Settings::GetListenPort() const
{
    return _settings->value("ethernet/listen_port", 8080).toUInt();
}

QString Settings::GetDatabaseName() const
{
    return _settings->value("database/name", "server.db").toString();
}

void Settings::SetDatabaseName(const QString& name)
{
    _settings->setValue("database/name", name);
}

int Settings::GetKeyExpirationTime() const
{
    return _settings->value("database/key_expiration_time", 30).toInt();
}

bool Settings::GetEnableCrypto() const
{
    return _settings->value("crypto/enable", true).toBool();
}
