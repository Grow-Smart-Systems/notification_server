#include "HTTPPacket.h"

HTTPPacket::HTTPPacket(QString message)
    : QObject{nullptr}
    , _message(message)
{
}

HTTPPacket::HTTPPacket(const HTTPPacket &old) : QObject(nullptr)
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

const QString &HTTPPacket::message() const
{
    return _message;
}

void HTTPPacket::setMessage(const QString &newMessage)
{
    _message = newMessage;
}

HTTPPacket::METHOD HTTPPacket::method() const
{
    return _method;
}

const QString &HTTPPacket::versionHttp() const
{
    return _versionHttp;
}

const QString &HTTPPacket::userAgentString() const
{
    return _userAgentString;
}

const QHostAddress &HTTPPacket::hostAddress() const
{
    return _hostAddress;
}

const QStringList &HTTPPacket::acceptLanguageList() const
{
    return _acceptLanguageList;
}

const QStringList &HTTPPacket::acceptEncodingList() const
{
    return _acceptEncodingList;
}

const QStringList &HTTPPacket::acceptList() const
{
    return _acceptList;
}

bool HTTPPacket::parse()
{
    if(_message.isEmpty())
        return false;

    _message =_message.trimmed();
    QStringList lines = _message.split("\n");
    for(const auto& line : lines)
    {
        //GET header
        if(line.contains("GET ", Qt::CaseInsensitive))
        {
            _method = METHOD::GET;

            QStringList header = line.split(" ");
            if(header.count() < 3)
                return false;

            // request
            _requestString = header.at(1);
            if(!parseRequestLine(_requestString))
                return false;

        }
        else if(line.contains("HOST:", Qt::CaseInsensitive))
        {
            QStringList addressList = line.split(":");
            if(addressList.count() < 2)
                return false;

            _hostAddress.setAddress(addressList.at(0));
            _hostPort = addressList.at(1).toUInt();
        }
        else if(line.contains("User-Agent:", Qt::CaseInsensitive))
        {
            _userAgentString = QString(line).remove("User-Agent:");
        }
        else if(line.contains("Accept:", Qt::CaseInsensitive))
        {
            _acceptList = QString(line).remove("Accept:").split(",");
        }
        else if(line.contains("Accept-Encoding:", Qt::CaseInsensitive))
        {
            _acceptEncodingList = QString(line).remove("Accept-Encoding:").split(",");
        }
        else if(line.contains("Accept-Language:", Qt::CaseInsensitive))
        {
            _acceptLanguageList = QString(line).remove("Accept-Language:").split(",");
        }

    }

    return true;
}

bool HTTPPacket::parse(const QString &message)
{
    setMessage(message);
    return parse();
}

const QHash<QString, QString> &HTTPPacket::requestParameters() const
{
    return _requestParameters;
}

const QString &HTTPPacket::requestStringPath() const
{
    return _requestStringPath;
}

const QString &HTTPPacket::requestString() const
{
    return _requestString;
}

quint16 HTTPPacket::hostPort() const
{
    return _hostPort;
}

bool HTTPPacket::parseRequestLine(QString requestLine)
{
    if(requestLine.isEmpty())
        return false;
    // разделитель ? - признак параметров
    QStringList requestLineList;
    if(!split(requestLine, requestLineList, "?"))
        return false;

    _requestStringPath = requestLineList.at(0);

    if(requestLineList.count() > 1)
    {
        //  разделитель параметров - &
        QStringList paramsList;
        if(!split(requestLineList.at(1), paramsList, "&"))
            return false;

        QStringList paramsAndValuesList;
        for(const auto& param : paramsList)
        {
            if(!split(param, paramsAndValuesList, "=", 2))
                return false;

            _requestParameters.insert(paramsAndValuesList.at(0), paramsAndValuesList.at(1));

            paramsAndValuesList.clear();
        }
    }
    return true;
}

bool HTTPPacket::split(QString string, QStringList &splittedStringList, const QString &splitter, const uint countCheck)
{
    if(splitter.isEmpty())
        return false;

    if(string.contains(splitter))
    {
        splittedStringList = string.split(splitter);
    }
    else
    {
        splittedStringList.append(string);
    }

    if(countCheck > 0 && splittedStringList.count() < countCheck)
        return false;
    return true;

}
