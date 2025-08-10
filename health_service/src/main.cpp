#include <iostream>
#include <thread>
#include <chrono>

#include "PingSender.h"
#include "ServiceList.h"
#include "AsyncLogger.h"

#define DEFAULT_REQUEST_INTERVAL 3
#define DEFAULT_LOG_FILE "/app/health_service.log"

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

std::string GetLogFilePathFromEnv()
{
    const char* log_file = std::getenv("LOG_FILE_PATH");
    if (log_file)
    {
        std::cout << "Используемый путь к лог-файлу: " << log_file << std::endl;
        return std::string(log_file);
    }
    return DEFAULT_LOG_FILE;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    std::cout << "Запуск службы проверки состояния сервисов..." << std::endl;

    // Получаем интервал запросов из переменной окружения
    g_logger = new AsyncLogger(GetLogFilePathFromEnv());
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
            delete g_logger;
            g_logger = nullptr;
            return 1;
        }

        std::vector<std::thread> threads;
        for (auto& service : services) 
        {
            threads.emplace_back([&pingSender, &service]()
            {
                pingSender.SendPing(service);
            });
        }
        for (auto& t : threads)
        {
            if (t.joinable()) 
                t.join();
        }
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }

    delete g_logger;
    g_logger = nullptr;
    
    return 0;
}