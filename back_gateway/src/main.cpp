// Minimal Hello World используя userver core (без HTTP сервера).
// Демонстрирует запуск корутины через RunStandalone.

#include <iostream>
#include <userver/engine/run_standalone.hpp>

int main(int /*argc*/, char* /*argv*/[]) 
{
    userver::engine::RunStandalone(1, [] {
        std::cout << "Hello, world from back_gateway (userver core)!" << std::endl;
    });
    return 0;
}

