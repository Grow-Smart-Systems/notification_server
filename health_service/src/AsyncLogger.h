#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <thread>


/// @brief Асинхронный потокобезопасный логгер.
/// @note Запускает отдельный поток для записи сообщений в лог-файл.
/// Рабочие потоки только добавляют сообщения через log().
class AsyncLogger final 
{
public:
    /// @brief Конструктор логгера
    /// @param filename Путь к лог-файлу
    AsyncLogger(const std::string& filename);

    /// @brief Деструктор логгера
    ~AsyncLogger();

    /// @brief Добавить сообщение в лог
    /// @param msg Строка для записи
    void log(const std::string& msg);

    /// @brief Запрет копирования
    AsyncLogger(const AsyncLogger&) = delete;

    /// @brief Запрет перемещения
    AsyncLogger& operator=(const AsyncLogger&) = delete;

private:
    /// @brief Реализация (pImpl idiom)
    struct Impl
    {
        /// @brief Конструктор
        Impl(const std::string& filename);

        /// @brief Деструктор
        ~Impl();

        /// @brief Обработчик сообщений
        void process();

        /// @brief Лог-файл
        std::ofstream log_file;

        /// @brief Очередь сообщений
        std::queue<std::string> messages;

        /// @brief Мьютекс для синхронизации доступа к очереди сообщений
        std::mutex mtx;

        /// @brief Условная переменная для синхронизации
        std::condition_variable cv;

        /// @brief Флаг, указывающий на состояние работы
        std::atomic<bool> running{ true };

        /// @brief Рабочий поток
        std::thread worker;
    };

    /// @brief Указатель на реализацию
    Impl* pImpl;
};

/// @brief Глобальный указатель на логгер
extern AsyncLogger* g_logger;
