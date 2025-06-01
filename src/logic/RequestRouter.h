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
        //! @param packet Объект HTTPPacket с разобранным HTTP-запросом
        void OnNewRequest(const Ethernet::TCPSocketKey& key, 
                          const Ethernet::HTTPPacket& packet);

    private:
        //! @brief Указатель на прикладной уровень (ApplicationLayer)
        QSharedPointer<Ethernet::ApplicationLayer> _applicationLayer {nullptr};
    };
}; // namespace Logic

#endif // REQUESTROUTER_H


/*TODO: организовать обмен ключами по следующей схеме

Пример процесса (для симметричного шифрования или CBC (Cipher Block Chaining)): 
Устройство и сервер изначально имеют общий ключ K1.
Сервер генерирует новый ключ K2.
Сервер шифрует K2 с помощью K1 (AES-CBC) и добавляет HMAC(K1, K2).
Сервер отправляет зашифрованный K2 и HMAC устройству через интернет (например, по TCP).
Устройство расшифровывает K2 с помощью K1, проверяет HMAC.
Если проверка успешна, устройство заменяет K1 на K2.
Устройство отправляет серверу зашифрованное подтверждение с использованием K2.
*/