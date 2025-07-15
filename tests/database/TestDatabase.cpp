#include <QtTest/QtTest>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "Settings.h"
#include "Database.h"

class TestDatabase : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    //void testAddUser();
    // void testAddDevice();
    // void testCreateConnectionKey();
    // void testSetPublicKeyForDevice();
    // void testCheckModelVersion();
    // void testGetPrivateKeyByClientKey();
    // void testGetPublicClientKeyByGUID();

private:
    //QString testDbName = ":memory:";
    QString testDbName = "test_db.sqlite"; // Имя тестовой базы данных
};

// ====================================================================================

void TestDatabase::initTestCase()
{
    Settings::GetInstance()->SetDatabaseName(testDbName);

    // Таблицы создаются автоматически через Connection
    Database::Database::GetInstance();
    }

void TestDatabase::cleanupTestCase()
{
    //QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

// void TestDatabase::testAddUser()
// {
//     Database::Contexts::ContextAddUser ctx;
//     ctx.username = "testuser";
//     ctx.password = "testpass";
//     ctx.email = "test@example.com";

//     auto& db = Database::Database::GetInstance();
//     QVERIFY(db.AddUser(ctx));
// }

// void TestDatabase::testAddDevice()
// {
//     Database::Contexts::ContextAddDevice ctx;
//     ctx.userID = 1;
//     ctx.deviceType = Database::Contexts::BaseContextDevice::DeviceType::SmokeDetectorV1;
//     ctx.ipAddress = "127.0.0.1";
//     ctx.guid = "test-guid-1234";

//     auto& db = Database::Database::GetInstance();
//     QVERIFY(db.AddDevice(ctx));
// }

// void TestDatabase::testCreateConnectionKey()
// {
//     Database::Contexts::ContextCreateKey ctx;
//     ctx.deviceID = 1;
//     ctx.userID = 1;
//     ctx.publicKey = QByteArray("112233");
//     ctx.privateKey = QByteArray("445566");

//     auto& db = Database::Database::GetInstance();
//     QVERIFY(db.CreateConnectionKey(ctx));
// }

// void TestDatabase::testSetPublicKeyForDevice()
// {
//     Database::Contexts::ContextSetPublicKeyForDevice ctx;
//     ctx.deviceID = 1;
//     ctx.clientPublicKey = QByteArray("778899");

//     auto& db = Database::Database::GetInstance();
//     QVERIFY(db.SetPublicKeyForDevice(ctx));
// }

// void TestDatabase::testCheckModelVersion()
// {
//     Database::Contexts::ContextCheckModelVersion ctx;
//     ctx.version = 1;

//     auto& db = Database::Database::GetInstance();
//     QVERIFY(db.CheckModelVersion(ctx));
// }

// void TestDatabase::testGetPrivateKeyByClientKey()
// {
//     Database::Contexts::ContextGetPrivateKeyByClientKey ctx;
//     ctx.publicKey = QByteArray("112233");

//     auto& db = Database::Database::GetInstance();
//     QVERIFY(db.GetPrivateKeyByClientKey(ctx));
// }

// void TestDatabase::testGetPublicClientKeyByGUID()
// {
//     Database::Contexts::ContextGetPublicClientKeyByGUID ctx;
//     ctx.deviceGuid = GUID{ "test-guid-1234" };

//     auto& db = Database::Database::GetInstance();
//     QVERIFY(db.GetPublicClientKeyByGUID(ctx));
// }

QTEST_MAIN(TestDatabase)
#include "TestDatabase.moc"