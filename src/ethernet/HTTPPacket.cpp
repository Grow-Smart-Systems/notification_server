#include "HTTPPacket.h"

HTTPPacket::HTTPPacket(QString message)
    : QObject{ nullptr }
    , _message(message)
{
}

HTTPPacket::HTTPPacket(const HTTPPacket& old) : QObject(nullptr)
{
    setParent(old.parent());

    _message = old.message();
    _method = old.method();
    _versionHttp = old.versionHttp();
    _requestString = old.requestString();
    _requestStringPath = old.requestStringPath();
    _requestParameters = old.requestParameters();
    _hostAddress = old.hostAddress();
    _hostPort = old.hostPort();
    _userAgentString = old.userAgentString();
    _acceptList = old.acceptList();
    _acceptEncodingList = old.acceptEncodingList();
    _acceptLanguageList = old.acceptLanguageList();
}

const QString& HTTPPacket::message() const
{
    return _message;
}

void HTTPPacket::setMessage(const QString& newMessage)
{
    _message = newMessage;
}

HTTPPacket::METHOD HTTPPacket::method() const
{
    return _method;
}

const QString& HTTPPacket::versionHttp() const
{
    return _versionHttp;
}

const QString& HTTPPacket::userAgentString() const
{
    return _userAgentString;
}

const QHostAddress& HTTPPacket::hostAddress() const
{
    return _hostAddress;
}

const QStringList& HTTPPacket::acceptLanguageList() const
{
    return _acceptLanguageList;
}

const QStringList& HTTPPacket::acceptEncodingList() const
{
    return _acceptEncodingList;
}

const QStringList& HTTPPacket::acceptList() const
{
    return _acceptList;
}

bool HTTPPacket::parse()
{
    if (_message.isEmpty())
        return false;

    _message = _message.trimmed();
    QStringList lines = _message.split("\n", Qt::SkipEmptyParts);
    if (lines.isEmpty())
        return false;

    // Парсим первую строку (request line)
    QString requestLine = lines.first().trimmed();
    QStringList requestParts = requestLine.split(" ", Qt::SkipEmptyParts);
    if (requestParts.size() < 3)
        return false;

    QString methodStr = requestParts.at(0).trimmed().toUpper();
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

    // Парсим заголовки
    for (int i = 1; i < lines.size(); ++i)
    {
        QString line = lines.at(i).trimmed();
        if (line.isEmpty())
            continue;
        if (line.startsWith("Host:", Qt::CaseInsensitive))
        {
            QString hostValue = line.mid(5).trimmed();
            int colonIdx = hostValue.lastIndexOf(":");
            if (colonIdx > 0) {
                QString host = hostValue.left(colonIdx).trimmed();
                QString portStr = hostValue.mid(colonIdx + 1).trimmed();
                _hostAddress.setAddress(host);
                _hostPort = portStr.toUShort();
            }
            else {
                _hostAddress.setAddress(hostValue);
                _hostPort = 80; // по умолчанию
            }
        }
        else if (line.startsWith("User-Agent:", Qt::CaseInsensitive))
        {
            _userAgentString = line.mid(11).trimmed();
        }
        else if (line.startsWith("Accept:", Qt::CaseInsensitive))
        {
            QString accept = line.mid(7).trimmed();
            _acceptList = accept.split(",", Qt::SkipEmptyParts);
            for (auto& s : _acceptList) s = s.trimmed();
        }
        else if (line.startsWith("Accept-Encoding:", Qt::CaseInsensitive))
        {
            QString acceptEnc = line.mid(15).trimmed();
            _acceptEncodingList = acceptEnc.split(",", Qt::SkipEmptyParts);
            for (auto& s : _acceptEncodingList) s = s.trimmed();
        }
        else if (line.startsWith("Accept-Language:", Qt::CaseInsensitive))
        {
            QString acceptLang = line.mid(15).trimmed();
            _acceptLanguageList = acceptLang.split(",", Qt::SkipEmptyParts);
            for (auto& s : _acceptLanguageList) s = s.trimmed();
        }
        // Можно добавить обработку других заголовков при необходимости
    }
    return true;
}

bool HTTPPacket::parse(const QString& message)
{
    setMessage(message);
    return parse();
}

const QHash<QString, QString>& HTTPPacket::requestParameters() const
{
    return _requestParameters;
}

const QString& HTTPPacket::requestStringPath() const
{
    return _requestStringPath;
}

const QString& HTTPPacket::requestString() const
{
    return _requestString;
}

quint16 HTTPPacket::hostPort() const
{
    return _hostPort;
}

bool HTTPPacket::parseRequestLine(QString requestLine)
{
    if (requestLine.isEmpty())
        return false;
    // разделитель ? - признак параметров
    QStringList requestLineList;
    if (!split(requestLine, requestLineList, "?"))
        return false;

    _requestStringPath = requestLineList.at(0);

    if (requestLineList.count() > 1)
    {
        //  разделитель параметров - &
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

bool HTTPPacket::split(QString string, QStringList& splittedStringList, const QString& splitter, const uint countCheck)
{
    if (splitter.isEmpty())
        return false;

    if (string.contains(splitter))
    {
        splittedStringList = string.split(splitter);
    }
    else
    {
        splittedStringList.append(string);
    }

    if (countCheck > 0 && splittedStringList.count() < countCheck)
        return false;
    return true;

}
