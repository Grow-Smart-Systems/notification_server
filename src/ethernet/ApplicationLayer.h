#ifndef APPLICATIONLAYER_H
#define APPLICATIONLAYER_H

#include <QObject>
#include <QRegularExpression>

#include "TransportLayer.h"
#include "HTTPPacket.h"


class ApplicationLayer final : public QObject
{
    Q_OBJECT
public:
    //! \brief Конструктор класса прикладного уровня получения
    explicit ApplicationLayer(QObject *parent = nullptr);

    /*!
     * \brief Инициализирует экземпляр ApplicationLayer.
     * 
     * Этот метод отвечает за настройку ApplicationLayer, включая инициализацию
     * транспортного уровня и настройку необходимых соединений. Его следует вызывать
     * после создания объекта и до его использования.
     * 
     * \return Возвращает true, если инициализация прошла успешно, иначе false.
     */
    bool init();

protected slots:

    /*!
     * \brief Обрабатывает получение нового сообщения от транспортного уровня.
     * 
     * Этот слот подключается к сигналу от транспортного уровня, который испускается
     * каждый раз при получении нового сообщения. Он отвечает за дальнейшую обработку
     * сообщения, такую как извлечение HTTP-запросов.
     * 
     * \param msg Сообщение, полученное от транспортного уровня, в виде QString.
     */
    void onNewMessage(QString msg);

private:
    //! \brief Указатель на транспортный уровень
    QSharedPointer<TransportLayer> _transportLayer;

    //! \brief Объект выделения GET пакетов
    QRegularExpression _re;

    //! \brief Функция выделение запроса из всего сообщения
    QString getRequestFromMessage(QString message);

signals:
    //! \brief Сигнал на получения запроса
    void signalNewRequest(HTTPPacket);

};

#endif // APPLICATIONLAYER_H
