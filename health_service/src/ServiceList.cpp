#include "ServiceList.h"

#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#define DEFAULT_JSON_PATH "/app/health_service/services.json"

void ServiceList::UpdateServiceInfo()
{
    if (_jsonPath.empty())
    {
        const char* json_path = std::getenv("SERVICES_JSON_PATH");
        _jsonPath = json_path ? json_path : DEFAULT_JSON_PATH;
        std::cout << "Используемый путь к JSON-файлу: " << _jsonPath << std::endl;
    }
    _services = GetServiceListFromJson(_jsonPath);
}

std::vector<ServiceInfo>& ServiceList::GetServiceInfo() noexcept
{
    return _services;
}

std::vector<ServiceInfo> ServiceList::GetServiceListFromJson(const std::string& path)
{
    std::vector<ServiceInfo> result;
    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        std::cerr << "Не удалось открыть файл сервисов: " << path << std::endl;
        return result;
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    if (!doc.IsArray())
    {
        std::cerr << "JSON должен быть массивом объектов!" << std::endl;
        return result;
    }

    for (const auto& item : doc.GetArray())
    {
        if (item.HasMember("name") &&
            item.HasMember("address") &&
            item.HasMember("port"))
        {
            result.push_back({
                item["name"].GetString(),
                item["address"].GetString(),
                item["port"].GetInt()
                });
        }
    }
    return result;
}