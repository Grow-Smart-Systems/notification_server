#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include "Defines.h"

namespace Crypto
{
    //! @brief Интерфейсный класс для расшифровки и шифрования сообщений
    //! @details формат зашифрованных сообщений: "[Длина ключа|Ключ|Сообщение]"
    class Crypto final : public QObject
    {
        Q_OBJECT
    public:
        //! @brief Возвращает экземпляр класса KeyGen
        static Crypto& getInstance()
        {
            static Crypto instance;
            return instance;
        }

        //! @brief Расшифровывает сообщение
        //! @param message Зашифрованное сообщение в формате 
        //! "[Длина ключа|Ключ|Сообщение]"
        //! @return Пара GUID устройства и расшифрованное сообщение
        QPair<QString, QString> DecryptMessage(const QByteArray& message);

        QByteArray EncryptMessage(const GUID& guid, const QString& message);

    private:
        //! @brief Конструктор по умолчанию
        Crypto() = default;

        //! @brief Деструктор
        ~Crypto() = default;

        Crypto(const Crypto&) = delete;
        Crypto(Crypto&&) = delete;
        Crypto& operator=(const Crypto&) = delete;
        Crypto& operator=(Crypto&&) = delete;
    };
};
#endif // ENCRYPTOR_H