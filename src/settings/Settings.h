#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>


class Settings final
{
public:
    ///@brief Получить экземпляр класса
    ///@return Указатель на экземпляр класса
    static Settings* getInstance()
    {
        static Settings instance;
        return &instance;
    }

private:
        ///@brief Конструктор
        Settings() = default;

        ///@brief Деструктор
        ~Settings() = default;


    

};

#endif // SETTINGS_H