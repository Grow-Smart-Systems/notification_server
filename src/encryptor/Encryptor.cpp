#include "Encryptor.h"

namespace Crypto
{
    QByteArray Encryptor::Encrypt(const QByteArray& data, const QByteArray& key)
    {
        return data; // Заглушка, замените на реальную реализацию
    }

    QByteArray Encryptor::Decrypt(const QByteArray& data, const QByteArray& key)
    {
        return data; // Заглушка, замените на реальную реализацию
    }

    QByteArray Encryptor::GenerateIV()
    {
        return QByteArray(); // Заглушка, замените на реальную реализацию
    }
}; // namespace Crypto