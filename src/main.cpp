#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[]) 
{
    QCoreApplication app(argc, argv);

    qDebug() << "Notification server started.";

    return app.exec();
}