#include "HTTPPacket.h"

HTTPPacket::HTTPPacket(const QString& message)
    : QObject{ nullptr }
    , _message(message)
{
}

HTTPPacket::HTTPPacket(const HTTPPacket& old) : QObject(nullptr)
{
    setParent(old.parent());
    _message = old.Message();
    _method = old.Method();
    _versionHttp = old.VersionHttp();
    _requestString = old.RequestString();
    _requestStringPath = old.RequestStringPath();
    _requestParameters = old.RequestParameters();
    _hostAddress = old.HostAddress();
    _hostPort = old.HostPort();
    _userAgentString = old.UserAgentString();
    _acceptList = old.AcceptList();
    _acceptEncodingList = old.AcceptEncodingList();
    _acceptLanguageList = old.AcceptLanguageList();
}

QString HTTPPacket::Message() const
{
    return _message;
}

void HTTPPacket::SetMessage(const QString& newMessage)
{
    _message = newMessage;
}

HTTPPacket::METHOD HTTPPacket::Method() const
{
    return _method;
}

QString HTTPPacket::VersionHttp() const
{
    return _versionHttp;
}

QString HTTPPacket::UserAgentString() const
{
    return _userAgentString;
}

QHostAddress HTTPPacket::HostAddress() const
{
    return _hostAddress;
}

QStringList HTTPPacket::AcceptLanguageList() const
{
    return _acceptLanguageList;
}

QStringList HTTPPacket::AcceptEncodingList() const
{
    return _acceptEncodingList;
}

QStringList HTTPPacket::AcceptList() const
{
    return _acceptList;
}

quint16 HTTPPacket::HostPort() const
{
    return _hostPort;
}

QString HTTPPacket::RequestString() const
{
    return _requestString;
}

QString HTTPPacket::RequestStringPath() const
{
    return _requestStringPath;
}

QHash<QString, QString> HTTPPacket::RequestParameters() const
{
    return _requestParameters;
}

bool HTTPPacket::Parse()
{
    if (_message.isEmpty())
        return false;
    _message = _message.trimmed();
    const QStringList lines = _message.split("\n", Qt::SkipEmptyParts);
    if (lines.isEmpty())
        return false;

    // Парсим первую строку (request line)
    const QStringList requestParts = lines.first().trimmed().split(" ", Qt::SkipEmptyParts);
    if (requestParts.size() < 3)
        return false;

    const QString methodStr = requestParts.at(0).trimmed().toUpper();
    if (methodStr == "GET")
        _method = METHOD::GET;
    else if (methodStr == "POST")
        _method = METHOD::POST;
    else
        return false;

    _requestString = requestParts.at(1).trimmed();
    _versionHttp = requestParts.at(2).trimmed();
    if (!_requestString.isEmpty())
        parseRequestLine(_requestString);

    // Сбросим старые значения
    _hostAddress = QHostAddress();
    _hostPort = 0;
    _userAgentString.clear();
    _acceptList.clear();
    _acceptEncodingList.clear();
    _acceptLanguageList.clear();

    // Используем QHash для быстрого поиска нужных заголовков
    QHash<QString, QString> headers;
    for (int i = 1; i < lines.size(); ++i) {
        const QString line = lines.at(i).trimmed();
        if (line.isEmpty())
            continue;
        int colonIdx = line.indexOf(":");
        if (colonIdx > 0) {
            QString key = line.left(colonIdx).trimmed().toLower();
            QString value = line.mid(colonIdx + 1).trimmed();
            headers.insert(key, value);
        }
    }

    // Host
    if (headers.contains("host")) {
        const QString hostValue = headers["host"];
        int colonIdx = hostValue.lastIndexOf(":");
        if (colonIdx > 0) {
            const QString host = hostValue.left(colonIdx).trimmed();
            const QString portStr = hostValue.mid(colonIdx + 1).trimmed();
            _hostAddress.setAddress(host);
            _hostPort = portStr.toUShort();
        } else {
            _hostAddress.setAddress(hostValue);
            _hostPort = 80;
        }
    }
    // User-Agent
    if (headers.contains("user-agent"))
        _userAgentString = headers["user-agent"];
    // Accept
    if (headers.contains("accept")) {
        _acceptList = headers["accept"].split(",", Qt::SkipEmptyParts);
        for (QString& s : _acceptList) s = s.trimmed();
    }
    // Accept-Encoding
    if (headers.contains("accept-encoding")) {
        _acceptEncodingList = headers["accept-encoding"].split(",", Qt::SkipEmptyParts);
        for (QString& s : _acceptEncodingList) s = s.trimmed();
    }
    // Accept-Language
    if (headers.contains("accept-language")) {
        _acceptLanguageList = headers["accept-language"].split(",", Qt::SkipEmptyParts);
        for (QString& s : _acceptLanguageList) s = s.trimmed();
    }
    return true;
}

bool HTTPPacket::Parse(QString message)
{
    SetMessage(message);
    return Parse();
}

bool HTTPPacket::parseRequestLine(const QString& requestLine)
{
    if (requestLine.isEmpty())
        return false;

    QStringList requestLineList;
    if (!split(requestLine, requestLineList, "?"))
        return false;

    _requestStringPath = requestLineList.at(0);
    if (requestLineList.count() > 1)
    {
        QStringList paramsList;
        if (!split(requestLineList.at(1), paramsList, "&"))
            return false;

        QStringList paramsAndValuesList;
        for (const auto& param : paramsList)
        {
            if (!split(param, paramsAndValuesList, "=", 2))
                return false;
                
            _requestParameters.insert(paramsAndValuesList.at(0), paramsAndValuesList.at(1));
            paramsAndValuesList.clear();
        }
    }
    return true;
}

bool HTTPPacket::split(QString string, QStringList& splittedStringList, QString splitter, uint countCheck)
{
    if (splitter.isEmpty())
        return false;

    if (string.contains(splitter))
        splittedStringList = string.split(splitter);
    else
        splittedStringList.append(string);
        
    if (countCheck > 0 && splittedStringList.count() < (int)countCheck)
        return false;
    return true;
}
