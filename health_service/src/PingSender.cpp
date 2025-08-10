#include "PingSender.h"

#include <fstream>
#include <grpcpp/grpcpp.h>
#include "../build/protoc/health_service.grpc.pb.h"

#define LOG_FILE_PATH "/app/health_service.log"

void PingSender::SendPing(ServiceInfo& address) 
{
    if (address.address.empty() || address.port <= 0) 
    {
        std::cerr << "Invalid service address or port: " << address.name << std::endl;
        return;
    }

    std::string fullAddress = address.address + ":" + std::to_string(address.port);
    auto channel = grpc::CreateChannel(fullAddress, grpc::InsecureChannelCredentials());
    std::unique_ptr<HealthService::Stub> stub = HealthService::NewStub(channel);

    PingRequest request;
    request.set_message("ping");

    PingReply reply;
    grpc::ClientContext context;
    
    auto status = stub->Ping(&context, request, &reply);
    std::ofstream log(LOG_FILE_PATH, std::ios::app);
    if (status.ok()) 
    {
        log << "[" << fullAddress << "] Ответ: " << reply.message() << std::endl;
    } 
    else 
    {
        log << "[" << fullAddress << "] Ошибка: " << status.error_message() << std::endl;
    }
}