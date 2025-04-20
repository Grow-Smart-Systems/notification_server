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
        GET,
        POST
    };

    HTTPPacket(QString message = "");
    HTTPPacket(const HTTPPacket& old);

    const QString &message() const;

    void setMessage(const QString &newMessage);

    METHOD method() const;

    const QString &versionHttp() const;

    const QString &userAgentString() const;

    const QHostAddress &hostAddress() const;

    const QStringList &acceptLanguageList() const;

    const QStringList &acceptEncodingList() const;

    const QStringList &acceptList() const;

    quint16 hostPort() const;


    //! \brief Возвращает строку запроса
    const QString &requestString() const;

    //! \brief Возвращает путь из строки запроса
    const QString &requestStringPath() const;

    //! \brief Возвращает параметры запроса в виде хеш-таблицы
    const QHash<QString, QString> &requestParameters() const;

public slots:
    //! \brief Функция разбора сообщения, использующая внутреннее состояние
    bool parse();

    //! \brief Функция разбора сообщения из параметра
    bool parse(const QString& message);

private:
    //! \brief Неразобранный запрос (как есть)
    QString _message;

    //! \brief Тип запроса
    METHOD _method{METHOD::GET};

    //! \brief Номер версии HTTP
    QString _versionHttp;

    //! \brief Строка запроса
    QString _requestString;
    QString _requestStringPath;
    QHash<QString/*parameter*/,QString/*value*/> _requestParameters;

    //! \brief Параметр HOST:
    //! \brief Хост, к которому идёт обращение
    QHostAddress _hostAddress;

    //! \brief порт, по которому идёт обращение
    quint16 _hostPort {0};

    //! \brief Параметр User-Agent:
    //! \brief характеристики клиента, с которого осуществляется выход, например название и версия браузера
    QString _userAgentString;

    //! \brief Параметр Accept:
    //! \brief определяет, какие типы данных может обработать клиент
    QStringList _acceptList;

    //! \brief Параметр Accept-Encoding:
    //! \brief обозначает форматы и кодировку файлов
    QStringList _acceptEncodingList;

    //! \brief Параметр Accept-Language:
    //! \brief указывает на принимаемые языки
    QStringList _acceptLanguageList;

    //! \brief Функция разбора строки запроса в теле http заголовка
    bool parseRequestLine(QString requestLine);

    //! \brief Функция разделитель по строке с проверкой
    bool split(QString string, QStringList& splittedStringList, const QString& splitter, const uint countCheck = 0);

};

#endif // HTTPPACKET_H
