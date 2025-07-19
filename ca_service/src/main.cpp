#pragma once

#include <userver/components/component.hpp>

#include "message_receiver.h"


int main(int argc, char* argv[]) 
{
    std::cout << "Starting Notification Server..." << std::endl;
    sleep(10); // Задержка для демонстрации запуска
    return 0;
}
