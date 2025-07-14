#ifndef HTTPPACKET_H
#define HTTPPACKET_H

#include <QObject>
#include <QHostAddress>

namespace Ethernet
{
    class HTTPPacket final : public QObject
    {
        Q_OBJECT

        /// \brief Максимальный размер HTTP-запроса
        static constexpr int MAX_REQUEST_SIZE = 8192;

    public:
        //! @brief Enum для определения метода HTTP запроса
        enum class METHOD
        {
            GET,  // HTTP GET запрос
            POST  // HTTP POST запрос
        };

        //! @brief Enum для версии HTTP
        enum class VERSION
        {
            HTTP_1_0, // HTTP/1.0
            HTTP_1_1, // HTTP/1.1
            HTTP_2_0, // HTTP/2.0
            UNKNOWN   // Неизвестная версия
        };

        //! @brief Конструктор по умолчанию
        //! Создает пустой HTTPPacket
        HTTPPacket();

        //! @brief Конструктор класса HTTPPacket
        //! @param message Исходное сообщение HTTP (по умолчанию пустое)
        HTTPPacket(const QString& message = "");

        //! @brief Конструктор копирования
        //! @param old Существующий объект HTTPPacket
        HTTPPacket(const HTTPPacket& old);

        //! @brief Возвращает исходное HTTP-сообщение
        //! @return Строка исходного сообщения
        QString Message() const;

        //! @brief Устанавливает новое HTTP-сообщение
        //! @param newMessage Новое сообщение
        void SetMessage(const QString& newMessage);

        //! @brief Возвращает метод HTTP-запроса (GET/POST)
        //! @return Значение enum METHOD
        METHOD Method() const;

        //! @brief Устанавливает метод HTTP-запроса
        //! @param method Новый метод (GET/POST)
        void SetMethod(METHOD method);

        //! @brief Возвращает версию HTTP
        //! @return Значение enum VERSION
        VERSION VersionHttp() const;
        
        //! @brief Устанавливает версию HTTP
        //! @param version Новая версия HTTP (HTTP_1_0, HTTP_1_1, HTTP_2_0)
        void SetVersionHttp(VERSION version);

        //! @brief Возвращает строку User-Agent
        //! @return Строка User-Agent
        QString UserAgentString() const;

        //! @brief Устанавливает строку User-Agent
        //! @param userAgent Новая строка User-Agent
        void SetUserAgentString(const QString& userAgent);

        //! \brief Возвращает адрес хоста
        //! \return QHostAddress хоста
        QHostAddress HostAddress() const;

        //! \brief Устанавливает адрес хоста
        //! \param hostAddress Новый адрес хоста
        void SetHostAddress(const QHostAddress& hostAddress);

        //! \brief Возвращает список поддерживаемых языков (Accept-Language)
        //! \return QStringList языков
        QStringList AcceptLanguageList() const;

        //! \brief Устанавливает список поддерживаемых языков
        //! \param acceptLanguage Новый список языков
        void SetAcceptLanguageList(const QStringList& acceptLanguage);

        //! \brief Возвращает список поддерживаемых кодировок (Accept-Encoding)
        //! \return QStringList кодировок
        QStringList AcceptEncodingList() const;
        
        //! \brief Устанавливает список поддерживаемых кодировок
        //! \param acceptEncoding Новый список кодировок
        void SetAcceptEncodingList(const QStringList& acceptEncoding);

        //! \brief Возвращает список поддерживаемых типов данных (Accept)
        //! \return QStringList типов
        QStringList AcceptList() const;

        //! \brief Устанавливает список поддерживаемых типов данных
        //! \param accept Новый список типов данных
        void SetAcceptList(const QStringList& accept);

        //! \brief Возвращает порт хоста
        //! \return Порт (quint16)
        quint16 HostPort() const;

        //! \brief Устанавливает порт хоста
        //! \param hostPort Новый порт хоста
        void SetHostPort(quint16 hostPort);

        //! \brief Возвращает строку запроса (Request-Line)
        //! \return Строка запроса
        QString RequestString() const;

        //! \brief Устанавливает строку запроса
        //! \param requestString Новая строка запроса
        void SetRequestString(const QString& requestString);

        //! \brief Возвращает путь из строки запроса
        //! \return Строка пути
        QString RequestStringPath() const;

        //! \brief Устанавливает путь из строки запроса
        //! \param requestStringPath Новый путь
        void SetRequestStringPath(const QString& requestStringPath);

        //! \brief Возвращает параметры запроса в виде хеш-таблицы
        //! \return QHash параметров
        QHash<QString, QString> RequestParameters() const;

        //! \brief Устанавливает параметры запроса
        //! \param requestParameters Новые параметры запроса
        void SetRequestParameters(const QHash<QString, QString>& requestParameters);

        //! \brief Возвращает список строк тела запроса
        //! \return QStringList строк тела
        QStringList Body() const;

        //! \brief Устанавливает список строк тела запроса
        //! \return Строка тела запроса в формате HTTP
        QString BodyString() const;

        //! \brief Устанавливает список строк тела запроса
        //! \param body Новый список строк тела
        void SetBody(const QStringList& body);

        //! \brief Устанавливает строку тела запроса в формате HTTP
        //! \param bodyString Новая строка тела запроса
        void SetBodyString(const QString& bodyString);

    public slots:
        //! \brief Разбирает текущее сообщение, используя внутреннее состояние
        //! \return true, если разбор успешен
        bool Parse();

        //! \brief Разбирает сообщение, переданное в параметре
        //! \param message HTTP-сообщение для разбора
        //! \return true, если разбор усп(ешен
        bool Parse(QString message);

        //! \brief Формирует строку запроса на основе текущего состояния
        //! \return Строка запроса в формате HTTP
        QString Build();

    protected:
        //! \brief Функция разбора строки запроса в теле http заголовка
        //! \param requestLine Строка запроса
        //! \return true, если разбор успешен
        bool parseRequestLine(const QString& requestLine);

        //! \brief Функция-разделитель по строке с проверкой
        //! \param string Исходная строка
        //! \param splittedStringList Список для результата
        //! \param splitter Разделитель
        //! \param countCheck Ожидаемое количество частей (0 — не проверять)
        //! \return true, если разделение успешно
        bool split(QString string, QStringList& splittedStringList, QString splitter, uint countCheck = 0);

        //! \brief Функция для преобразования метода в строку
        //! \param method Метод HTTP
        //! \return Строка, представляющая метод
        QString versionToString(VERSION version) const;

    private:
        //! \brief Неразобранный запрос (как есть)
        QString _message;

        //! \brief Тип запроса (GET/POST)
        METHOD _method{ METHOD::GET };

        //! \brief Версия HTTP
        VERSION _versionHttp{ VERSION::UNKNOWN };

        //! \brief Строка запроса (Request-Line)
        QString _requestString;

        //! \brief Путь из строки запроса
        QString _requestStringPath;

        //! \brief Параметры запроса
        QHash<QString/*parameter*/, QString/*value*/> _requestParameters;

        //! \brief Хост, к которому идёт обращение
        QHostAddress _hostAddress { QHostAddress::Null };

        //! \brief Порт, по которому идёт обращение
        quint16 _hostPort{ 0 };

        //! \brief Строка User-Agent
        QString _userAgentString;

        //! \brief Список поддерживаемых типов данных (Accept)
        QStringList _acceptList;

        //! \brief Список поддерживаемых кодировок (Accept-Encoding)
        QStringList _acceptEncodingList;

        //! \brief Список поддерживаемых языков (Accept-Language)
        QStringList _acceptLanguageList;

        //! \brief Список строк тела запроса
        QStringList _body;

    };
};

#endif // HTTPPACKET_H
