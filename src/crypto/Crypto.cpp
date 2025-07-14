#include "Crypto.h"

#include "KeyWorker.h"
#include "Database.h"
#include "ContextGetPrivateKey.h"

namespace Crypto
{
    QPair<QString, QString> Crypto::DecryptMessage(const QByteArray& message)
    {
        if (message.isEmpty() || message.size() < 2)
        {
            qWarning() << "Message is empty.";
            return {};
        }

        // Выделим ключ и зашифрованное сообщение
        // используя длину-префикс - [2 байта длина ключа][ключ][сообщение]
        auto keyLength = static_cast<quint16>(message[0]) << 8 | static_cast<quint16>(message[1]);
        if (keyLength < 2 || keyLength > message.size() - 2)
        {
            qWarning() << "Invalid key length in message.";
            return {};
        }

        auto key = message.mid(2, keyLength);
        auto encryptedMessage = message.mid(2 + keyLength);
        if (key.isEmpty() || encryptedMessage.isEmpty())
        {
            qWarning() << "Key or encrypted message is empty.";
            return {};
        }

        // Получим ключ из базы данных
        Database::Contexts::ContextGetPrivateKeyByClientKey context;
        context.publicKey = key;
        try
        {
            if (!Database::Database::GetInstance().GetPrivateKeyByClientKey(context))
                return {};
        }
        catch(...)
        {
            qWarning() << "Error getting private key";
            return {};
        }
        
        if (context.privateKey.isEmpty())
        {
            qWarning() << "Private key not found for client:" << key.toHex();
            return {};
        }

        // Расшифруем данные с использованием ключа
        QByteArray data = KeyWorker::DecryptWithPrivateKey(encryptedMessage,
                                                           context.privateKey);
        return {context.GUID, QString::fromUtf8(data)};
    }
    QByteArray Crypto::EncryptMessage(const GUID& guid, const QString& message)
    {
        // Получим публичный ключ из базы данных
        Database::Contexts::ContextGetPublicClientKeyByGUID context;
        context.deviceGuid = guid;
        try
        {
            Database::Database::GetInstance().GetPublicClientKeyByGUID(context);
        }
        catch(...)
        {
            qWarning() << "Error getting public key";
            return {};
        }

        if (context.clientPublicKey.isEmpty())
        {
            qWarning() << "Public key not found for device:" << guid.toQString();
            return {};
        }

        // Зашифруем данные с использованием ключа
        QByteArray encryptedData = KeyWorker::EncryptWithPublicKey(message.toUtf8(), context.clientPublicKey);
        QByteArray result;
        result.append(static_cast<char>(context.clientPublicKey.size() >> 8));
        result.append(static_cast<char>(context.clientPublicKey.size() & 0xFF));
        result.append(context.clientPublicKey);
        result.append(encryptedData);
        return result;
    }
}; // namespace Crypto