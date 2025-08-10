#include "AsyncLogger.h"


AsyncLogger::Impl::Impl(const std::string& filename)
    : log_file(filename, std::ios::app)
{
    worker = std::thread([this]()
        {
            this->process();
        });
}

AsyncLogger::Impl::~Impl()
{
    running = false;
    cv.notify_all();
    if (worker.joinable()) 
        worker.join();
        
    // Записать оставшиеся сообщения
    while (!messages.empty())
    {
        log_file << messages.front() << std::endl;
        messages.pop();
    }
}

void AsyncLogger::Impl::process()
{
    std::unique_lock<std::mutex> lock(mtx);
    while (running)
    {
        cv.wait(lock, [this]()
            {
                return !messages.empty() || !running;
            });
        while (!messages.empty())
        {
            log_file << messages.front() << std::endl;
            messages.pop();
        }
    }
}

AsyncLogger* g_logger = nullptr;

AsyncLogger::AsyncLogger(const std::string& filename)
{
    pImpl = new Impl(filename);
}

AsyncLogger::~AsyncLogger()
{
    delete pImpl;
}
void AsyncLogger::log(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(pImpl->mtx);
    pImpl->messages.push(msg);
    pImpl->cv.notify_one();
}
