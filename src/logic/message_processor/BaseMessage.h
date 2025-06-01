#ifndef BASEMESSAGE_H
#define BASEMESSAGE_H

#include "Defines.h"
#include "HTTPPacket.h"

namespace Logic::MessageFactory
{
    /*! \brief Базовый класс для обработки сообщений.
     *  Реализует общий шаблон этапов обработки сообщения: валидация, сохранение, обработка, формирование ответа и логирование.
     */
    class BaseMessage : public QObject
    {
        Q_OBJECT
    public:
        /*! \brief Конструктор класса BaseMessage.
         *  \param key Ключ сокета, по которому пришло сообщение.
         *  \param packet Объект HTTPPacket, содержащий разобранный HTTP-запрос.
         */
        BaseMessage(const Ethernet::TCPSocketKey& key, const Ethernet::HTTPPacket& packet)
            : _socketKey(std::move(key))
            , _packet(std::move(packet))
        {};

        /*! \brief Виртуальный деструктор. */
        virtual ~BaseMessage() = default;

        /*! \brief Запускает обработку сообщения по этапам.
         *  Вызовы этапов: валидация, сохранение исходных данных, бизнес-логика, сохранение результата, формирование ответа, логирование.
         */
        virtual void HandleMassage()
        {
            ValidateAndParse();
            SaveRawData();
            ProcessBusinessLogic();
            SaveProcessedData();
            FormResponse();
            LogResult();
        }

    protected:
        /*! \brief Валидация и парсинг входных данных. */
        virtual void ValidateAndParse() {}

        /*! \brief Сохранение исходных (сырых) данных. */
        virtual void SaveRawData() {}

        /*! \brief Основная бизнес-логика обработки сообщения. */
        virtual void ProcessBusinessLogic() {}

        /*! \brief Сохранение обработанных данных. */
        virtual void SaveProcessedData() {}

        /*! \brief Формирование ответа для клиента. */
        virtual void FormResponse() {}

        /*! \brief Логирование результата обработки. */
        virtual void LogResult() {}

    protected:
        /*! \brief Ключ сокета, по которому пришло сообщение. */
        Ethernet::TCPSocketKey _socketKey;

        /*! \brief HTTP-пакет, содержащий разобранный запрос. */
        Ethernet::HTTPPacket _packet;

    signals:
        void SendResponse(const Ethernet::TCPSocketKey& key, const QString& response);
    };
};
#endif // BASEMESSAGE_H