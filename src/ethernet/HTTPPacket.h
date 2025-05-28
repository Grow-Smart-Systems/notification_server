#ifndef HTTPPACKET_H
#define HTTPPACKET_H

#include <QObject>
#include <QHostAddress>

class HTTPPacket final : public QObject
{
    Q_OBJECT
public:
    //! \brief Enum для определения метода HTTP запроса
    enum class  METHOD
    {
        GET,  //!< HTTP GET запрос
        POST  //!< HTTP POST запрос
    };

    //! \brief Конструктор класса HTTPPacket
    //! \param message Исходное сообщение HTTP (по умолчанию пустое)
    HTTPPacket(const QString& message = "");

    //! \brief Конструктор копирования
    //! \param old Существующий объект HTTPPacket
    HTTPPacket(const HTTPPacket& old);

    //! \brief Возвращает исходное HTTP-сообщение
    //! \return Строка исходного сообщения
    QString Message() const;

    //! \brief Устанавливает новое HTTP-сообщение
    //! \param newMessage Новое сообщение
    void SetMessage(const QString& newMessage);

    //! \brief Возвращает метод HTTP-запроса (GET/POST)
    //! \return Значение enum METHOD
    METHOD Method() const;

    //! \brief Возвращает версию HTTP
    //! \return Строка с версией HTTP
    QString VersionHttp() const;

    //! \brief Возвращает строку User-Agent
    //! \return Строка User-Agent
    QString UserAgentString() const;

    //! \brief Возвращает адрес хоста
    //! \return QHostAddress хоста
    QHostAddress HostAddress() const;

    //! \brief Возвращает список поддерживаемых языков (Accept-Language)
    //! \return QStringList языков
    QStringList AcceptLanguageList() const;

    //! \brief Возвращает список поддерживаемых кодировок (Accept-Encoding)
    //! \return QStringList кодировок
    QStringList AcceptEncodingList() const;

    //! \brief Возвращает список поддерживаемых типов данных (Accept)
    //! \return QStringList типов
    QStringList AcceptList() const;

    //! \brief Возвращает порт хоста
    //! \return Порт (quint16)
    quint16 HostPort() const;

    //! \brief Возвращает строку запроса (Request-Line)
    //! \return Строка запроса
    QString RequestString() const;

    //! \brief Возвращает путь из строки запроса
    //! \return Строка пути
    QString RequestStringPath() const;

    //! \brief Возвращает параметры запроса в виде хеш-таблицы
    //! \return QHash параметров
    QHash<QString, QString> RequestParameters() const;

public slots:
    //! \brief Разбирает текущее сообщение, используя внутреннее состояние
    //! \return true, если разбор успешен
    bool Parse();

    //! \brief Разбирает сообщение, переданное в параметре
    //! \param message HTTP-сообщение для разбора
    //! \return true, если разбор успешен
    bool Parse(QString message);

private:
    //! \brief Неразобранный запрос (как есть)
    QString _message;

    //! \brief Тип запроса (GET/POST)
    METHOD _method{METHOD::GET};

    //! \brief Номер версии HTTP
    QString _versionHttp;

    //! \brief Строка запроса (Request-Line)
    QString _requestString;
    
    //! \brief Путь из строки запроса
    QString _requestStringPath;

    //! \brief Параметры запроса
    QHash<QString/*parameter*/, QString/*value*/> _requestParameters;

    //! \brief Хост, к которому идёт обращение
    QHostAddress _hostAddress;

    //! \brief Порт, по которому идёт обращение
    quint16 _hostPort {0};

    //! \brief Строка User-Agent
    QString _userAgentString;

    //! \brief Список поддерживаемых типов данных (Accept)
    QStringList _acceptList;

    //! \brief Список поддерживаемых кодировок (Accept-Encoding)
    QStringList _acceptEncodingList;

    //! \brief Список поддерживаемых языков (Accept-Language)
    QStringList _acceptLanguageList;

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

};

#endif // HTTPPACKET_H
