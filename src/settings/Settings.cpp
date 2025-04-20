#include "Settings.h"

void Settings::sync()
{
    if(_settings)
        _settings->sync();
}

QDir Settings::getConfigDir() const
{
    return QDir(_settings->value("modems/config_dir", "configs").toString());
}

quint16 Settings::getListenPort() const
{
    return _settings->value("ethernet/listen_port", 8080).toUInt();
}
