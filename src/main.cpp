#include <QCoreApplication>
#include <QDebug>
#include "settings/Settings.h"
#include "logic/MainController.h"

class MainController;

int main(int argc, char *argv[]) 
{
    QCoreApplication app(argc, argv);

    qDebug() << "Notification server started.";
    auto settings = Settings::GetInstance();

    // Создание главного управляющего класса
    MainController mainController;

    return app.exec();
}