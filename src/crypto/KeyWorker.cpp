#include "KeyWorker.h"

#include <QtCrypto>

QCA::Initializer init;

namespace Crypto
{
    bool KeyWorker::GenerateKeyPair(QByteArray& publicKeyPem, QByteArray& privateKeyPem, int bits)
    {
        QCA::Initializer init;
        if (!QCA::isSupported("rsa"))
            qFatal("RSA is not supported by QCA");

        QCA::KeyGenerator KeyWorker;
        QCA::PrivateKey priv = KeyWorker.createRSA(bits);
        if (!priv.isNull())
        {
            QCA::PublicKey pub = priv.toPublicKey();
            publicKeyPem = pub.toPEM().toUtf8();
            privateKeyPem = priv.toPEM().toUtf8();
            return true;
        }
        return false;
    }

    QByteArray KeyWorker::EncryptWithPublicKey(const QByteArray& data, const QByteArray& publicKeyPem)
    {
        QCA::Initializer init;
        QCA::PublicKey pub = QCA::PublicKey::fromPEM(QString::fromUtf8(publicKeyPem));
        if (pub.isNull())
            return {};

        QCA::SecureArray plain(data);
        QCA::SecureArray encrypted = pub.encrypt(plain, QCA::EME_PKCS1_OAEP);
        return QByteArray(encrypted.data(), encrypted.size());
    }

    QByteArray KeyWorker::DecryptWithPrivateKey(const QByteArray& encrypted, const QByteArray& privateKeyPem)
    {
        QCA::Initializer init;
        QCA::PrivateKey priv = QCA::PrivateKey::fromPEM(QString::fromUtf8(privateKeyPem));
        if (priv.isNull())
            return {};

        QCA::SecureArray enc = QCA::SecureArray(encrypted);
        QCA::SecureArray plain;
        bool ok = priv.decrypt(enc, &plain, QCA::EME_PKCS1_OAEP);
        if (!ok)
            return {};
        return QByteArray(plain.data(), plain.size());
    }
}; // namespace Crypto