#ifndef KEYWORKER_H
#define KEYWORKER_H

#include <QObject>

namespace Crypto
{
    //! @brief Класс для работы с криптографическими ключами
    class KeyWorker final: public QObject
    {
        Q_OBJECT

    public:
        //! @brief Генерация пары ключей (PEM)
        //! @param publicKeyPem Ссылка на QByteArray для хранения открытого ключа в формате PEM
        //! @param privateKeyPem Ссылка на QByteArray для хранения закрытого ключа в формате PEM
        //! @param bits Количество бит для ключа (по умолчанию 2048)
        //! @return true, если ключи успешно сгенерированы, иначе false
        //! @details Генерирует пару ключей RSA и сохраняет их в формате PEM
        //! @details Если bits меньше 512, будет использовано значение по умолчанию
        static bool GenerateKeyPair(QByteArray& publicKeyPem, QByteArray& privateKeyPem, int bits = 2048);

        //! @brief Зашифровать данные открытым ключом (PEM)
        //! @param data Данные для шифрования
        //! @param publicKeyPem Открытый ключ в формате PEM
        //! @return Зашифрованные данные в формате QByteArray
        static QByteArray EncryptWithPublicKey(const QByteArray& data, const QByteArray& publicKeyPem);

        //! @brief Расшифровать данные закрытым ключом (PEM)
        //! @param encrypted Зашифрованные данные в формате QByteArray
        //! @param privateKeyPem Закрытый ключ в формате PEM
        //! @return Расшифрованные данные в формате QByteArray
        static QByteArray DecryptWithPrivateKey(const QByteArray& encrypted, const QByteArray& privateKeyPem);

    private:
        // Запрет на создание экземпляров класса
        KeyWorker(const KeyWorker&) = delete;
        KeyWorker(KeyWorker&&) = delete;
        KeyWorker& operator=(const KeyWorker&) = delete;
        KeyWorker& operator=(KeyWorker&&) = delete;
    };
} // namespace Crypto

#endif // KEYWORKER_H