#include "MainController.h"

MainController::MainController(QObject *parent)
    : QObject(parent)
{
    // Инициализация и связывание компонентов
    _requestRouter = QSharedPointer<RequestRouter>::create();
}