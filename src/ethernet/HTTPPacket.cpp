#include "HTTPPacket.h"

namespace Ethernet
{
    HTTPPacket::HTTPPacket(const QString& message)
        : QObject{ nullptr }
        , _message(message)
    {
    }

    HTTPPacket::HTTPPacket()
        : QObject{ nullptr }
        , _message(QString())
    {
    }

    HTTPPacket::HTTPPacket(const HTTPPacket& old) : QObject(nullptr)
    {
        setParent(old.parent());
        _message = old.Message();
        _method = old.Method();
        _versionHttp = old.VersionHttp(); // old.VersionHttp() теперь возвращает VERSION
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

    void HTTPPacket::SetMethod(METHOD method)
    {
        _method = method;
    }

    HTTPPacket::VERSION HTTPPacket::VersionHttp() const
    {
        return _versionHttp;
    }

    void HTTPPacket::SetVersionHttp(VERSION version)
    {
        _versionHttp = version;
    }

    QString HTTPPacket::UserAgentString() const
    {
        return _userAgentString;
    }

    void HTTPPacket::SetUserAgentString(const QString& userAgent)
    {
        _userAgentString = userAgent;
    }

    QHostAddress HTTPPacket::HostAddress() const
    {
        return _hostAddress;
    }

    void HTTPPacket::SetHostAddress(const QHostAddress& hostAddress)
    {
        _hostAddress = hostAddress;
        _hostPort = hostAddress.toIPv4Address() ? 80 : 0; // Устанавливаем порт по умолчанию
    }

    QStringList HTTPPacket::AcceptLanguageList() const
    {
        return _acceptLanguageList;
    }

    void HTTPPacket::SetAcceptLanguageList(const QStringList& acceptLanguage)
    {
        _acceptLanguageList = acceptLanguage;
        for (QString& s : _acceptLanguageList) s = s.trimmed();
    }

    QStringList HTTPPacket::AcceptEncodingList() const
    {
        return _acceptEncodingList;
    }

    void HTTPPacket::SetAcceptEncodingList(const QStringList& acceptEncoding)
    {
        _acceptEncodingList = acceptEncoding;
        for (QString& s : _acceptEncodingList) s = s.trimmed();
    }

    QStringList HTTPPacket::AcceptList() const
    {
        return _acceptList;
    }

    void HTTPPacket::SetAcceptList(const QStringList& accept)
    {
        _acceptList = accept;
        for (QString& s : _acceptList) s = s.trimmed();
    }

    quint16 HTTPPacket::HostPort() const
    {
        return _hostPort;
    }

    void HTTPPacket::SetHostPort(quint16 port)
    {
        _hostPort = port;
    }

    QString HTTPPacket::RequestString() const
    {
        return _requestString;
    }

    void HTTPPacket::SetRequestString(const QString& requestString)
    {
        _requestString = requestString;
        parseRequestLine(_requestString);
    }

    QString HTTPPacket::RequestStringPath() const
    {
        return _requestStringPath;
    }

    void HTTPPacket::SetRequestStringPath(const QString& requestStringPath)
    {
        _requestStringPath = requestStringPath;
        // Если путь не содержит параметров, очищаем их
        if (!_requestStringPath.contains("?"))
            _requestParameters.clear();
        else
            parseRequestLine(_requestStringPath);
    }

    QHash<QString, QString> HTTPPacket::RequestParameters() const
    {
        return _requestParameters;
    }

    QStringList HTTPPacket::Body() const
    {
        return _body;
    }

    QString HTTPPacket::BodyString() const
    {
        return _body.join("\r\n");
    }

    void HTTPPacket::SetBody(const QStringList& body)
    {
        _body = body;
    }

    void HTTPPacket::SetBodyString(const QString& bodyString)
    {
        _body = bodyString.split("\r\n", Qt::SkipEmptyParts);
    }

    bool HTTPPacket::Parse()
    {
        if (_message.isEmpty() ||
            _message.size() > MAX_REQUEST_SIZE)
            return false;

        // Удаляем лишние пробелы и разделяем на строки
        _message = _message.simplified();
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
        const QString versionStr = requestParts.at(2).trimmed();
        if (versionStr == "HTTP/1.0")
            _versionHttp = VERSION::HTTP_1_0;
        else if (versionStr == "HTTP/1.1")
            _versionHttp = VERSION::HTTP_1_1;
        else if (versionStr == "HTTP/2.0" || versionStr == "HTTP/2")
            _versionHttp = VERSION::HTTP_2_0;
        else
            _versionHttp = VERSION::UNKNOWN;

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
        for (int i = 1; i < lines.size(); ++i)
        {
            const QString line = lines.at(i).trimmed();
            if (line.isEmpty())
                continue;
            int colonIdx = line.indexOf(":");
            if (colonIdx > 0)
            {
                QString key = line.left(colonIdx).trimmed().toLower();
                QString value = line.mid(colonIdx + 1).trimmed();
                headers.insert(key, value);
            }
        }

        // Host
        if (headers.contains("host"))
        {
            const QString hostValue = headers["host"];
            int colonIdx = hostValue.lastIndexOf(":");
            if (colonIdx > 0)
            {
                const QString host = hostValue.left(colonIdx).trimmed();
                const QString portStr = hostValue.mid(colonIdx + 1).trimmed();
                _hostAddress.setAddress(host);
                _hostPort = portStr.toUShort();
            }
            else
            {
                _hostAddress.setAddress(hostValue);
                _hostPort = 80;
            }
        }
        // User-Agent
        if (headers.contains("user-agent"))
            _userAgentString = headers["user-agent"];
        // Accept
        if (headers.contains("accept"))
        {
            _acceptList = headers["accept"].split(",", Qt::SkipEmptyParts);
            for (QString& s : _acceptList) s = s.trimmed();
        }
        // Accept-Encoding
        if (headers.contains("accept-encoding"))
        {
            _acceptEncodingList = headers["accept-encoding"].split(",", Qt::SkipEmptyParts);
            for (QString& s : _acceptEncodingList) s = s.trimmed();
        }
        // Accept-Language
        if (headers.contains("accept-language"))
        {
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

    QString HTTPPacket::versionToString(VERSION version) const
    {
        switch (version)
        {
        case VERSION::HTTP_1_0:
            return "HTTP/1.0";
        case VERSION::HTTP_1_1:
            return "HTTP/1.1";
        case VERSION::HTTP_2_0:
            return "HTTP/2.0";
        default: return "HTTP/1.1";
        }
    }

    QString HTTPPacket::Build()
    {
        QString versionStr = versionToString(_versionHttp);
        QString requestLine;
        switch (_method)
        {
        case METHOD::GET:
            requestLine = "GET " + _requestStringPath + " " + versionStr;
            break;
        case METHOD::POST:
            requestLine = "POST " + _requestStringPath + " " + versionStr;
            break;
        default:
            return QString();
        }

        // Добавляем параметры запроса, если они есть
        if (!_requestParameters.isEmpty())
        {
            QStringList paramsList;
            for (auto it = _requestParameters.constBegin(); it != _requestParameters.constEnd(); ++it)
            {
                paramsList.append(it.key() + "=" + it.value());
            }
            requestLine += "?" + paramsList.join("&");
        }

        QStringList headers;
        headers.append(requestLine);
        headers.append("Host: " + _hostAddress.toString() + ":" + QString::number(_hostPort));
        if (!_userAgentString.isEmpty())
            headers.append("User-Agent: " + _userAgentString);
        if (!_acceptList.isEmpty())
            headers.append("Accept: " + _acceptList.join(", "));
        if (!_acceptEncodingList.isEmpty())
            headers.append("Accept-Encoding: " + _acceptEncodingList.join(", "));
        if (!_acceptLanguageList.isEmpty())
            headers.append("Accept-Language: " + _acceptLanguageList.join(", "));

        headers.append("");
        headers.append(_body);
        return headers.join("\r\n");
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
};