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
