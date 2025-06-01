#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include "Defines.h"

namespace Crypto
{
    class Encryptor final : public QObject
    {
        Q_OBJECT
    public:
        //! @brief Конструктор класса Encryptor
        Encryptor() = default;

        //! @brief Деструктор класса Encryptor
        ~Encryptor() = default;

        //! @brief Шифрует данные с использованием симметричного шифрования
        static QString Encrypt(const QString& data);

        //! @brief Дешифрует данные с использованием симметричного шифрования
        static QString Decrypt(const QString& data);

    private:
        //! @brief Генерирует вектор инициализации (IV) для шифрования
        static QString GenerateIV();
    };
};
#endif // ENCRYPTOR_H