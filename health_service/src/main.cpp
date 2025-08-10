#include <iostream>
#include <thread>
#include <chrono>

#include "PingSender.h"
#include "ServiceList.h"

#define DEFAULT_REQUEST_INTERVAL 3

int GetIntervalFromEnv()
{
    const char* env_interval = std::getenv("REQUEST_INTERVAL");
    if (env_interval) 
    {
        try 
        {
            return std::stoi(env_interval);
        } 
        catch (...) 
        {
            return DEFAULT_REQUEST_INTERVAL;
        }
    }
    return DEFAULT_REQUEST_INTERVAL;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    std::cout << "Запуск службы проверки состояния сервисов..." << std::endl;

    // Получаем интервал запросов из переменной окружения
    int interval = GetIntervalFromEnv();

    ServiceList serviceList;
    PingSender pingSender;

    // Основной цикл запросов
    while (true) 
    {
        serviceList.UpdateServiceInfo();
        auto& services = serviceList.GetServiceInfo();
        if (services.empty()) 
        {
            std::cerr << "Список сервисов пуст или его не удалось загрузить!" << std::endl;
            return 1;
        }

        for (auto& service : services) 
        {
            pingSender.SendPing(service);
        }
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
    return 0;
}