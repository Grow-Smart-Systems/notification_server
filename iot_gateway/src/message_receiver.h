#pragma once

#include <userver/components/component.hpp>


class MessageReceiver final : public userver::components::ComponentBase
{
public:
    static constexpr std::string_view kName = "message-receiver";
    MessageReceiver(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context);
    // Здесь реализуйте методы для приёма сообщений
};
