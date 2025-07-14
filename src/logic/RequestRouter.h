#ifndef REQUESTROUTER_H
#define REQUESTROUTER_H

#include <QObject>

#include "HTTPPacket.h"
#include "ApplicationLayer.h"


namespace Logic
{
    //! @brief Класс RequestRouter отвечает за маршрутизацию и обработку HTTP-запросов,
    //! поступающих с прикладного уровня (ApplicationLayer).
    class RequestRouter final : public QObject
    {
        Q_OBJECT
    public:
        //! @brief Конструктор класса RequestRouter
        //! @param parent Родительский QObject
        explicit RequestRouter(QObject* parent = nullptr);

    public slots:
        //! @brief Слот для обработки новых HTTP-запросов от ApplicationLayer
        //! @param deviceGuid GUID устройства, отправившего запрос
        //! @param packet Объект HTTPPacket с разобранным HTTP-запросом
        void OnNewRequest(const GUID& deviceGuid, 
                          const Ethernet::HTTPPacket& packet);

    private:
        //! @brief Указатель на прикладной уровень (ApplicationLayer)
        QSharedPointer<Ethernet::ApplicationLayer> _applicationLayer {nullptr};
    };
}; // namespace Logic

#endif // REQUESTROUTER_H