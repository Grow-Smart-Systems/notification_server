#pragma once

#include <userver/components/component.hpp>

#include "message_receiver.h"


int main(int argc, char* argv[]) 
{
    return userver::components::Run(argc, argv, 
        {
            MessageReceiver::kName, MessageReceiver{}
            // Здесь добавьте остальные ваши компоненты
        });
}
