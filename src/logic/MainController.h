#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "RequestRouter.h"

/**
 * @brief Главный управляющий класс приложения.
 */
class MainController final : public QObject
{
    Q_OBJECT
    
public:
    /**
     * @brief Конструктор MainController
     * @param parent Родительский QObject
     */
    MainController(QObject *parent = nullptr);

    ~MainController() = default;

private:
    /**
     * @brief Роутер HTTP-запросов
     */
    QSharedPointer<RequestRouter> _requestRouter;
};

#endif // MAINCONTROLLER_H
