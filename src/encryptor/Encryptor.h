#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include "Defines.h"

namespace Crypto
{
    class Encryptor final : public QObject
    {
        Q_OBJECT
    public:
        //! \brief Конструктор класса Encryptor
        Encryptor() = default;

        //! \brief Деструктор класса Encryptor
        ~Encryptor() = default;

        //! \brief Шифрует данные с использованием симметричного шифрования
        static QByteArray Encrypt(const QByteArray& data, const QByteArray& key);

        //! \brief Дешифрует данные с использованием симметричного шифрования
        static QByteArray Decrypt(const QByteArray& data, const QByteArray& key);

    private:
        //! \brief Генерирует вектор инициализации (IV) для шифрования
        static QByteArray GenerateIV();
    };
};
#endif // ENCRYPTOR_H